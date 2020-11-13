#ifndef SERVER_H
#define SERVER_H

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include <memory.h>
#include <string.h>

#include "protocol2.h"
#include "Client.h"


using namespace protocol2;
using namespace network1;

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"

//class Sever
//{
//
//public:
//	Sever();
//	~Sever();
//	bool Oncreate();
//
//	bool InitializeNetwork();
//	void ShutdownNetwork();
//	bool IsNetworkInitialized();
//private:
//	WSADATA wsaData;
//	int iResult;
//
//	SOCKET ListenSocket = INVALID_SOCKET;
//	SOCKET ClientSocket = INVALID_SOCKET;
//
//	struct addrinfo* result = NULL;
//	struct addrinfo hints;
//
//	int iSendResult;
//	char recvbuf[DEFAULT_BUFLEN];
//	int recvbuflen = DEFAULT_BUFLEN;
//
//
//};



struct ServerClientData
{
    Address address;
    uint64_t clientSalt;
    uint64_t challengeSalt;
    double connectTime;
    double lastPacketSendTime;
    double lastPacketReceiveTime;

    ServerClientData()
    {
        clientSalt = 0;
        challengeSalt = 0;
        connectTime = 0.0;
        lastPacketSendTime = 0.0;
        lastPacketReceiveTime = 0.0;
    }
};

class Server
{
    Socket* m_socket;                                                  // socket for sending and receiving packets.

    PacketFactory * m_packetFactory;                                    // packet factory for creating and destroying packets.

    uint64_t m_serverSalt;                                              // server salt. randomizes hash keys to eliminate challenge/response hash worst case attack.

    int m_numConnectedClients;                                          // number of connected clients

    bool m_clientConnected[MaxClients];                                 // true if client n is connected

    uint64_t m_clientSalt[MaxClients];                                  // array of client salt values per-client

    uint64_t m_challengeSalt[MaxClients];                               // array of challenge salt values per-client

    Address m_clientAddress[MaxClients];                                // array of client address values per-client

    ServerClientData m_clientData[MaxClients];                          // heavier weight data per-client, eg. not for fast lookup


public:

    Server(Socket& socket, PacketFactory& packetFactory)
    {
        m_socket = &socket;
        m_packetFactory = &packetFactory;
        m_numConnectedClients = 0;
        for (int i = 0; i < MaxClients; ++i)
            ResetClientState(i);
    }

    ~Server()
    {
        assert(m_socket);
        assert(m_packetFactory);
        m_packetFactory = NULL;
        m_socket = NULL;
    }

    void SendPackets(double time)
    {
        for (int i = 0; i < MaxClients; ++i)
        {
            if (!m_clientConnected[i])
                continue;

            if (m_clientData[i].lastPacketSendTime + ConnectionKeepAliveSendRate > time)
                return;

            ConnectionKeepAlivePacket* packet = (ConnectionKeepAlivePacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);
            packet->client_salt = m_clientSalt[i];
            packet->challenge_salt = m_challengeSalt[i];

            SendPacketToConnectedClient(i, packet, time);
        }
    }

    void ReceivePackets(double time)
    {
        while (true)
        {
            Address address;
            Packet* packet = ReceivePacket(m_socket, m_packetFactory, address);
            if (!packet)
                break;

            switch (packet->GetType())
            {
            case PACKET_CONNECTION_REQUEST:
                ProcessConnectionRequest(*(ConnectionRequestPacket*)packet, address, time);
                break;

            case PACKET_CONNECTION_KEEP_ALIVE:
                ProcessConnectionKeepAlive(*(ConnectionKeepAlivePacket*)packet, address, time);
                break;

            case PACKET_CONNECTION_DISCONNECT:
                ProcessConnectionDisconnect(*(ConnectionDisconnectPacket*)packet, address, time);
                break;

            default:
                break;
            }

            //m_packetFactory->DestroyPacket(packet);
        }
    }

    void CheckForTimeOut(double time)
    {
        for (int i = 0; i < MaxClients; ++i)
        {
            if (!m_clientConnected[i])
                continue;

            if (m_clientData[i].lastPacketReceiveTime + KeepAliveTimeOut < time)
            {
                char buffer[256];
                const char* addressString = m_clientAddress[i].ToString(buffer, sizeof(buffer));
                printf("client %d timed out (client address = %s, client salt = %" PRIx64 ", challenge salt = %" PRIx64 ")\n", i, addressString, m_clientSalt[i], m_challengeSalt[i]);
                DisconnectClient(i, time);
            }
        }
    }

    int GetNumConnectedClients() const
    {
        return m_numConnectedClients;
    }

protected:

    Packet* ReceivePacket(Socket* socket, PacketFactory* packetFactory, Address& address)
    {
        uint8_t packetData[MaxPacketSize];

        int packetBytes = 0;
            //= socket->ReceivePacket(address, packetData, MaxPacketSize);

        if (!packetBytes)
            return NULL;

        protocol2::PacketInfo info;
        info.protocolId = ProtocolId;
        info.packetFactory = packetFactory;

        return protocol2::ReadPacket(info, packetData, packetBytes, NULL, NULL);
    }

    void ResetClientState(int clientIndex)
    {
        assert(clientIndex >= 0);
        assert(clientIndex < MaxClients);
        m_clientConnected[clientIndex] = false;
        m_clientSalt[clientIndex] = 0;
        m_challengeSalt[clientIndex] = 0;
        m_clientAddress[clientIndex] = Address();
        m_clientData[clientIndex] = ServerClientData();
    }

    int FindFreeClientIndex() const
    {
        for (int i = 0; i < MaxClients; ++i)
        {
            if (!m_clientConnected[i])
                return i;
        }
        return -1;
    }

    int FindExistingClientIndex(const Address& address, uint64_t clientSalt, uint64_t challengeSalt) const
    {
        for (int i = 0; i < MaxClients; ++i)
        {
            if (m_clientConnected[i] && m_clientAddress[i] == address && m_clientSalt[i] == clientSalt && m_challengeSalt[i] == challengeSalt)
                return i;
        }
        return -1;
    }

    void ConnectClient(int clientIndex, const Address& address, uint64_t clientSalt, uint64_t challengeSalt, double time)
    {
        assert(m_numConnectedClients >= 0);
        assert(m_numConnectedClients < MaxClients - 1);
        assert(!m_clientConnected[clientIndex]);

        m_numConnectedClients++;

        m_clientConnected[clientIndex] = true;
        m_clientSalt[clientIndex] = clientSalt;
        m_challengeSalt[clientIndex] = challengeSalt;
        m_clientAddress[clientIndex] = address;

        m_clientData[clientIndex].address = address;
        m_clientData[clientIndex].clientSalt = clientSalt;
        m_clientData[clientIndex].challengeSalt = challengeSalt;
        m_clientData[clientIndex].connectTime = time;
        m_clientData[clientIndex].lastPacketSendTime = time;
        m_clientData[clientIndex].lastPacketReceiveTime = time;

        char buffer[256];
        const char* addressString = address.ToString(buffer, sizeof(buffer));
        printf("client %d connected (client address = %s, client salt = %" PRIx64 ", challenge salt = %" PRIx64 ")\n", clientIndex, addressString, clientSalt, challengeSalt);

        ConnectionKeepAlivePacket* connectionKeepAlivePacket = (ConnectionKeepAlivePacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);
        connectionKeepAlivePacket->client_salt = m_clientSalt[clientIndex];
        connectionKeepAlivePacket->challenge_salt = m_challengeSalt[clientIndex];

        SendPacketToConnectedClient(clientIndex, connectionKeepAlivePacket, time);
    }

    void DisconnectClient(int clientIndex, double time)
    {
        assert(clientIndex >= 0);
        assert(clientIndex < MaxClients);
        assert(m_numConnectedClients > 0);
        assert(m_clientConnected[clientIndex]);

        char buffer[256];
        const char* addressString = m_clientAddress[clientIndex].ToString(buffer, sizeof(buffer));
        printf("client %d disconnected: (client address = %s, client salt = %" PRIx64 ", challenge salt = %" PRIx64 ")\n", clientIndex, addressString, m_clientSalt[clientIndex], m_challengeSalt[clientIndex]);

        ConnectionDisconnectPacket* packet = (ConnectionDisconnectPacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_DISCONNECT);
        packet->client_salt = m_clientSalt[clientIndex];
        packet->challenge_salt = m_challengeSalt[clientIndex];

        SendPacketToConnectedClient(clientIndex, packet, time);

        ResetClientState(clientIndex);

        m_numConnectedClients--;
    }

    bool IsConnected(const Address& address, uint64_t clientSalt) const
    {
        for (int i = 0; i < MaxClients; ++i)
        {
            if (!m_clientConnected[i])
                continue;
            if (m_clientAddress[i] == address && m_clientSalt[i] == clientSalt)
                return true;
        }
        return false;
    }

    void SendPacketToConnectedClient(int clientIndex, Packet* packet, double time)
    {
        assert(packet);
        assert(clientIndex >= 0);
        assert(clientIndex < MaxClients);
        assert(m_clientConnected[clientIndex]);
        m_clientData[clientIndex].lastPacketSendTime = time;
        SendPacket(m_socket, m_packetFactory, m_clientAddress[clientIndex], packet);
    }

    void ProcessConnectionRequest(const ConnectionRequestPacket& packet, const Address& address, double time)
    {
        //char buffer[256];
        //const char* addressString = address.ToString(buffer, sizeof(buffer));
        //printf("processing connection request packet from: %s\n", addressString);

        //if (m_numConnectedClients == MaxClients)
        //{
        //    printf("connection denied: server is full\n");
        //    ConnectionDeniedPacket* connectionDeniedPacket = (ConnectionDeniedPacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_DENIED);
        //    connectionDeniedPacket->client_salt = packet.client_salt;
        //    connectionDeniedPacket->reason = CONNECTION_DENIED_SERVER_FULL;
        //    SendPacket(m_socket, m_packetFactory, address, connectionDeniedPacket);
        //    return;
        //}

        //if (IsConnected(address, packet.client_salt))
        //{
        //    printf("connection denied: already connected\n");
        //    ConnectionDeniedPacket* connectionDeniedPacket = (ConnectionDeniedPacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_DENIED);
        //    connectionDeniedPacket->client_salt = packet.client_salt;
        //    connectionDeniedPacket->reason = CONNECTION_DENIED_ALREADY_CONNECTED;
        //    SendPacket(m_socket, m_packetFactory, address, connectionDeniedPacket);
        //    return;
        //}

        //ServerChallengeEntry* entry = FindOrInsertChallenge(address, packet.client_salt, time);
        //if (!entry)
        //    return;

        //assert(entry);
        //assert(entry->address == address);
        //assert(entry->client_salt == packet.client_salt);

        //if (entry->last_packet_send_time + ChallengeSendRate < time)
        //{
        //    printf("sending connection challenge to %s (challenge salt = %" PRIx64 ")\n", addressString, entry->challenge_salt);
        //    ConnectionChallengePacket* connectionChallengePacket = (ConnectionChallengePacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_CHALLENGE);
        //    connectionChallengePacket->client_salt = packet.client_salt;
        //    connectionChallengePacket->challenge_salt = entry->challenge_salt;
        //    SendPacket(m_socket, m_packetFactory, address, connectionChallengePacket);
        //    entry->last_packet_send_time = time;
        //}
    }

    //void ProcessConnectionResponse(const ConnectionResponsePacket& packet, const Address& address, double time)
    //{
    //    const int existingClientIndex = FindExistingClientIndex(address, packet.client_salt, packet.challenge_salt);
    //    if (existingClientIndex != -1)
    //    {
    //        assert(existingClientIndex >= 0);
    //        assert(existingClientIndex < MaxClients);

    //        if (m_clientData[existingClientIndex].lastPacketSendTime + ConnectionConfirmSendRate < time)
    //        {
    //            ConnectionKeepAlivePacket* connectionKeepAlivePacket = (ConnectionKeepAlivePacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);
    //            connectionKeepAlivePacket->client_salt = m_clientSalt[existingClientIndex];
    //            connectionKeepAlivePacket->challenge_salt = m_challengeSalt[existingClientIndex];
    //            SendPacketToConnectedClient(existingClientIndex, connectionKeepAlivePacket, time);
    //        }

    //        return;
    //    }

    //    char buffer[256];
    //    const char* addressString = address.ToString(buffer, sizeof(buffer));
    //    printf("processing connection response from client %s (client salt = %" PRIx64 ", challenge salt = %" PRIx64 ")\n", addressString, packet.client_salt, packet.challenge_salt);

    //    ServerChallengeEntry* entry = FindChallenge(address, packet.client_salt, time);
    //    if (!entry)
    //        return;

    //    assert(entry);
    //    assert(entry->address == address);
    //    assert(entry->client_salt == packet.client_salt);

    //    if (entry->challenge_salt != packet.challenge_salt)
    //    {
    //        printf("connection challenge mismatch: expected %" PRIx64 ", got %" PRIx64 "\n", entry->challenge_salt, packet.challenge_salt);
    //        return;
    //    }

    //    if (m_numConnectedClients == MaxClients)
    //    {
    //        if (entry->last_packet_send_time + ConnectionChallengeSendRate < time)
    //        {
    //            printf("connection denied: server is full\n");
    //            ConnectionDeniedPacket* connectionDeniedPacket = (ConnectionDeniedPacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_DENIED);
    //            connectionDeniedPacket->client_salt = packet.client_salt;
    //            connectionDeniedPacket->reason = CONNECTION_DENIED_SERVER_FULL;
    //            SendPacket(m_socket, m_packetFactory, address, connectionDeniedPacket);
    //            entry->last_packet_send_time = time;
    //        }
    //        return;
    //    }

    //    const int clientIndex = FindFreeClientIndex();

    //    assert(clientIndex != -1);
    //    if (clientIndex == -1)
    //        return;

    //    ConnectClient(clientIndex, address, packet.client_salt, packet.challenge_salt, time);
    //}

    void ProcessConnectionKeepAlive(const ConnectionKeepAlivePacket& packet, const Address& address, double time)
    {
        const int clientIndex = FindExistingClientIndex(address, packet.client_salt, packet.challenge_salt);
        if (clientIndex == -1)
            return;

        assert(clientIndex >= 0);
        assert(clientIndex < MaxClients);

        m_clientData[clientIndex].lastPacketReceiveTime = time;
    }

    void ProcessConnectionDisconnect(const ConnectionDisconnectPacket& packet, const Address& address, double time)
    {
        const int clientIndex = FindExistingClientIndex(address, packet.client_salt, packet.challenge_salt);
        if (clientIndex == -1)
            return;

        assert(clientIndex >= 0);
        assert(clientIndex < MaxClients);

        DisconnectClient(clientIndex, time);
    }

    void SendPacket(Socket* socket, PacketFactory* packetFactory, const Address& address, Packet* packet)
    {
        assert(socket);
        assert(packetFactory);
        assert(address.IsValid());
        assert(packet);

        uint8_t packetData[MaxPacketSize];

        protocol2::PacketInfo info;
        info.protocolId = ProtocolId;
        info.packetFactory = packetFactory;

        const int packetSize = 1;

        //= protocol2::WritePacket(info, packet, packetData, MaxPacketSize);

        if (packetSize > 0)
        {
            //socket->SendPacket(address, packetData, packetSize);
        }

        //packetFactory->DestroyPacket(packet);
    };
};


#endif // !SERVER_H

