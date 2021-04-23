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

#include "Client.h"
#include <sstream>
#include <algorithm>

using namespace protocol2;
using namespace network1;

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"



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

    ServerClientData m_clientData[MaxClients];                          // heavier weight data per-client, eg. not for fast lookup                       // heavier weight data per-client, eg. not for fast lookup

    ServerChallengeHash m_challengeHash;

    static std::unique_ptr<Server> ServerInstance;
    friend std::default_delete<Server>;

    std::vector<NetworkobjectsData> networkObjects;


public:
    Server(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator = (const Server&) = delete;
    Server& operator = (Server&&) = delete;
    static Server* getInstance();

    Server(Socket& socket, PacketFactory& packetFactory)
    {
        m_socket = &socket;
        m_packetFactory = &packetFactory;
        m_numConnectedClients = 0;
        for (int i = 0; i < MaxClients; ++i)
            ResetClientState(i);
    }    
    
    Server();

    ~Server()
    {
        assert(m_socket);
        assert(m_packetFactory);
        m_packetFactory = NULL;
        m_socket = NULL;
    }



    void setServerSocket(Socket& socket, PacketFactory& packetFactory)
    {
        m_socket = &socket;
        m_packetFactory = &packetFactory;
        m_numConnectedClients = 0;
        for (int i = 0; i < MaxClients; ++i)
            ResetClientState(i);
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
        Address address;
        Packet* packet = ReceivePacket(m_socket, m_packetFactory, address);

        if (!packet)
            return;

        switch (packet->GetType())
        {
        case PACKET_CONNECTION_REQUEST:
            ProcessConnectionRequest(*(ConnectionRequestPacket*)packet, address, time);
            break;

        case PACKET_CONNECTION_RESPONSE:
            ProcessConnectionResponse(*(ConnectionResponsePacket*)packet, address, time);
            break;

        case PACKET_CONNECTION_KEEP_ALIVE:
            ProcessConnectionKeepAlive(*(ConnectionKeepAlivePacket*)packet, address, time);
            break;

        case PACKET_CONNECTION_DISCONNECT:
            ProcessConnectionDisconnect(*(ConnectionDisconnectPacket*)packet, address, time);
            break;

        case TEST_PACKET_CREATEPLAYER:
            ProcessConnectionCreatePlayer(*(TestPacket*)packet, time);

            break;
        case PACKET_UPDATE_POSITION:
            ProcessUpdatePosition(*(UpdatePosition*)packet, time);
            break;

        case PACKET_JSON:
            ProcessPacketJson(*(JsonPacket*)packet, time);
            break;

        default:
            break;
        }

        m_packetFactory->DestroyPacket(packet);
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

    uint64_t GenerateSalt()
    {
        return ((uint64_t(rand()) << 0) & 0x000000000000FFFFull) |
            ((uint64_t(rand()) << 16) & 0x00000000FFFF0000ull) |
            ((uint64_t(rand()) << 32) & 0x0000FFFF00000000ull) |
            ((uint64_t(rand()) << 48) & 0xFFFF000000000000ull);
    };

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

    uint64_t CalculateChallengeHashKey(const Address& address, uint64_t clientSalt, uint64_t serverSeed)
    {
        char buffer[256];
        const char* addressString = address.ToString(buffer, sizeof(buffer));
        const int addressLength = (int)strlen(addressString);
        return murmur_hash_64(&serverSeed, 8, murmur_hash_64(&clientSalt, 8, murmur_hash_64(addressString, addressLength, 0)));
    }

    ServerChallengeEntry* FindChallenge(const Address& address, uint64_t clientSalt, double time)
    {
        const uint64_t key = CalculateChallengeHashKey(address, clientSalt, m_serverSalt);

        int index = key % ChallengeHashSize;

        printf("client salt = %" PRIx64 "\n", clientSalt);
        printf("challenge hash key = %" PRIx64 "\n", key);
        printf("challenge hash index = %d\n", index);

        if (m_challengeHash.exists[index] &&
            m_challengeHash.entries[index].client_salt == clientSalt &&
            m_challengeHash.entries[index].address == address &&
            m_challengeHash.entries[index].create_time + ChallengeTimeOut >= time)
        {
            printf("found challenge entry at index %d\n", index);

            return &m_challengeHash.entries[index];
        }

        return NULL;
    }

    ServerChallengeEntry* FindOrInsertChallenge(const Address& address, uint64_t clientSalt, double time)
    {
        const uint64_t key = CalculateChallengeHashKey(address, clientSalt, m_serverSalt);

        int index = key % ChallengeHashSize;

        printf("client salt = %" PRIx64 "\n", clientSalt);
        printf("challenge hash key = %" PRIx64 "\n", key);
        printf("challenge hash index = %d\n", index);

        if (!m_challengeHash.exists[index] || (m_challengeHash.exists[index] && m_challengeHash.entries[index].create_time + ChallengeTimeOut < time))
        {
            printf("found empty entry in challenge hash at index %d\n", index);

            ServerChallengeEntry* entry = &m_challengeHash.entries[index];

            entry->client_salt = clientSalt;
            entry->challenge_salt = GenerateSalt();
            entry->last_packet_send_time = time - ChallengeSendRate * 2;
            entry->create_time = time;
            entry->address = address;

            m_challengeHash.exists[index] = 1;

            return entry;
        }

        if (m_challengeHash.exists[index] &&
            m_challengeHash.entries[index].client_salt == clientSalt &&
            m_challengeHash.entries[index].address == address)
        {
            printf("found existing challenge hash entry at index %d\n", index);

            return &m_challengeHash.entries[index];
        }

        return NULL;
    }


    void ProcessConnectionRequest(const ConnectionRequestPacket& packet, const Address& address, double time)
    {
        char buffer[256];
        const char* addressString = address.ToString(buffer, sizeof(buffer));
        printf("processing connection request packet from: %s\n", addressString);

        if (m_numConnectedClients == MaxClients)
        {
            printf("connection denied: server is full\n");
            ConnectionDeniedPacket* connectionDeniedPacket = (ConnectionDeniedPacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_DENIED);
            connectionDeniedPacket->client_salt = packet.client_salt;
            connectionDeniedPacket->reason = CONNECTION_DENIED_SERVER_FULL;
            SendPacket(m_socket, m_packetFactory, address, connectionDeniedPacket);
            return;
        }

        if (IsConnected(address, packet.client_salt))
        {
            printf("connection denied: already connected\n");
            ConnectionDeniedPacket* connectionDeniedPacket = (ConnectionDeniedPacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_DENIED);
            connectionDeniedPacket->client_salt = packet.client_salt;
            connectionDeniedPacket->reason = CONNECTION_DENIED_ALREADY_CONNECTED;
            SendPacket(m_socket, m_packetFactory, address, connectionDeniedPacket);
            return;
        }

        ServerChallengeEntry* entry = FindOrInsertChallenge(address, packet.client_salt, time);
        if (!entry)
            return;

        assert(entry);
        assert(entry->address == address);
        assert(entry->client_salt == packet.client_salt);

        if (entry->last_packet_send_time + ChallengeSendRate < time)
        {
            printf("sending connection challenge to %s (challenge salt = %" PRIx64 ")\n", addressString, entry->challenge_salt);
            ConnectionChallengePacket* connectionChallengePacket = (ConnectionChallengePacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_CHALLENGE);
            connectionChallengePacket->client_salt = packet.client_salt;
            connectionChallengePacket->challenge_salt = entry->challenge_salt;
            SendPacket(m_socket, m_packetFactory, address, connectionChallengePacket);
            entry->last_packet_send_time = time;
        }
    }

    void ProcessConnectionResponse(const ConnectionResponsePacket& packet, const Address& address, double time)
    {
        const int existingClientIndex = FindExistingClientIndex(address, packet.client_salt, packet.challenge_salt);
        if (existingClientIndex != -1)
        {
            assert(existingClientIndex >= 0);
            assert(existingClientIndex < MaxClients);

            if (m_clientData[existingClientIndex].lastPacketSendTime + ConnectionConfirmSendRate < time)
            {
                ConnectionKeepAlivePacket* connectionKeepAlivePacket = (ConnectionKeepAlivePacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);
                connectionKeepAlivePacket->client_salt = m_clientSalt[existingClientIndex];
                connectionKeepAlivePacket->challenge_salt = m_challengeSalt[existingClientIndex];
                SendPacketToConnectedClient(existingClientIndex, connectionKeepAlivePacket, time);
            }

            return;
        }

        char buffer[256];
        const char* addressString = address.ToString(buffer, sizeof(buffer));
        printf("processing connection response from client %s (client salt = %" PRIx64 ", challenge salt = %" PRIx64 ")\n", addressString, packet.client_salt, packet.challenge_salt);

        ServerChallengeEntry* entry = FindChallenge(address, packet.client_salt, time);
        if (!entry)
            return;

        assert(entry);
        assert(entry->address == address);
        assert(entry->client_salt == packet.client_salt);

        if (entry->challenge_salt != packet.challenge_salt)
        {
            printf("connection challenge mismatch: expected %" PRIx64 ", got %" PRIx64 "\n", entry->challenge_salt, packet.challenge_salt);
            return;
        }

        if (m_numConnectedClients == MaxClients)
        {
            if (entry->last_packet_send_time + ConnectionChallengeSendRate < time)
            {
                printf("connection denied: server is full\n");
                ConnectionDeniedPacket* connectionDeniedPacket = (ConnectionDeniedPacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_DENIED);
                connectionDeniedPacket->client_salt = packet.client_salt;
                connectionDeniedPacket->reason = CONNECTION_DENIED_SERVER_FULL;
                SendPacket(m_socket, m_packetFactory, address, connectionDeniedPacket);
                entry->last_packet_send_time = time;
            }
            return;
        }

        const int clientIndex = FindFreeClientIndex();

        assert(clientIndex != -1);
        if (clientIndex == -1)
            return;

        ConnectClient(clientIndex, address, packet.client_salt, packet.challenge_salt, time);
    }

    void ProcessConnectionKeepAlive(const ConnectionKeepAlivePacket& packet, const Address& address, double time)
    {
        const int clientIndex = FindExistingClientIndex(address, packet.client_salt, packet.challenge_salt);
        if (clientIndex == -1)
            return;

        assert(clientIndex >= 0);
        assert(clientIndex < MaxClients);

        m_clientData[clientIndex].lastPacketReceiveTime = time;

        Server::getInstance()->SendPackets(time);
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
    
    void ProcessConnectionCreatePlayer(const TestPacket& packet, double time)
    {
        //List of player

        //NetworkobjectsData tmp;
        //tmp.modelName = (char*)packet.modelName;
        //tmp.tagName = (char*)packet.tagName;
        //tmp.position = packet.position;
        ////Add these infor into a map 

        //networkObjects.push_back(tmp);
        //if (std::find(networkObjects.begin(), networkObjects.end(), tmp) != networkObjects.end())
        //    std::cout << "Element found";
        //else
        //    networkObjects.push_back(tmp);

        //Use this list as a packect to send back to client


        //client will use this list create all player 

        //for (size_t i = 0; i < m_numConnectedClients; i++)
        //{
        //    for (size_t i = 0; i < networkObjects.size(); i++)
        //    {
        //        TestPacket* test = (TestPacket*)m_packetFactory->CreatePacket(TEST_PACKET_CREATEPLAYER);

        //        //test->setUpData((char*)packet.modelName, (char*)packet.tagName, packet.position);
        //        test->setUpData(networkObjects[i].modelName, networkObjects[i].tagName, networkObjects[i].position);

        //        SendPacketToConnectedClient(i, test, time);
        //    }
        //}

        //for (size_t i = 0; i < m_numConnectedClients; i++)
        //{
        //    TestPacket* test = (TestPacket*)m_packetFactory->CreatePacket(TEST_PACKET_CREATEPLAYER);

        //    
        //    //test->setUpData((char*)packet.modelName, (char*)packet.tagName, packet.position);

        //    for (size_t i = 0; i < networkObjects.size(); i++)
        //    {
        //        test->setupdata(networkObjects[i].modelName, networkObjects[i].tagName, networkObjects[i].position);
        //    }
        //    
        //    SendPacketToConnectedClient(i, test, time);

        //}
    }    
    
    void ProcessUpdatePosition(const UpdatePosition & packet, double time)
    {
        for (size_t i = 0; i < m_numConnectedClients; i++)
        {
            UpdatePosition* test = (UpdatePosition*)m_packetFactory->CreatePacket(PACKET_UPDATE_POSITION);

            test->setUpData((char*)packet.tagName, packet.position);

            SendPacketToConnectedClient(i, test, time);
        }
    }    
    
    void ProcessPacketJson(const JsonPacket& packet, double time)
    {
        json j;
        auto j3 = json::parse(packet.jsonString);

        NetworkobjectsData tmp;

        tmp.tagName = j3["tag"];
        tmp.modelName = j3["modelName"];

        std::vector<float> position;
        for (auto& elem : j3["position"])
        {
            position.push_back(elem);
        }

        if (position.size() != 0)
        {
            tmp.position = vec3(position[0], position[1], position[2]);
        }


        if (std::find(networkObjects.begin(), networkObjects.end(), tmp) != networkObjects.end())
            std::cout << "Element found";
        else 
            networkObjects.push_back(tmp);

        j["playerInfo"] = json::array();

        //MAKE it become a string again
        for (auto init : networkObjects )
        {
            j["playerInfo"].push_back({ {"tag", init.tagName}, {"modelName", init.modelName }, {"position", {init.position.x,init.position.y,init.position.z}} });
           //j["playerInfo"] = { {"tag", init.tagName}, {"modelName", init.modelName }, {"position", {init.position.x,init.position.y,init.position.z}} };
        }

        std::cout << "Json Send: " << j << std::endl;

        //MAKE json into string
        std::string s = j.dump();

        for (size_t i = 0; i < m_numConnectedClients; i++)
        {
            JsonPacket* test = (JsonPacket*)m_packetFactory->CreatePacket(PACKET_JSON);
            //setup packet Using string
            test->setUpData(s);
            SendPacketToConnectedClient(i, test, time);

        }
    }


    Packet* ReceivePacket(Socket* socket, PacketFactory* packetFactory, Address& address)
    {
        uint8_t packetData[MaxPacketSize];

        int packetBytes = socket->ReceivePacket(address, packetData, MaxPacketSize);

        if (!packetBytes)
            return NULL;

        protocol2::PacketInfo info;
        info.protocolId = ProtocolId;
        info.packetFactory = packetFactory;

        return protocol2::ReadPacket(info, packetData, packetBytes, NULL);
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

        const int packetSize = protocol2::WritePacket(info, packet, packetData, MaxPacketSize);

        if (packetSize > 0)
        {
            socket->SendPacket(address, packetData, packetSize);
        }

        packetFactory->DestroyPacket(packet);
    }


};


#endif // !SERVER_H

