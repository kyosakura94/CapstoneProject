#include <cassert>
#include "protocol2.h"


#include <inttypes.h>
#include "Socket.h";
#include "Address.h";
#include "network.h"


#ifndef CLIENT_H
#define CLIENT_H


using namespace protocol2;
using namespace network1;

enum PacketTypes
{
    PACKET_CONNECTION_REQUEST,                      // client requests a connection.
    PACKET_CONNECTION_DENIED,                       // server denies client connection request.
    PACKET_CONNECTION_CHALLENGE,                    // server response to client connection request.
    PACKET_CONNECTION_RESPONSE,                     // client response to server connection challenge.
    PACKET_CONNECTION_KEEP_ALIVE,                   // keep alive packet sent at some low rate (once per-second) to keep the connection alive
    PACKET_CONNECTION_DISCONNECT,                   // courtesy packet to indicate that the other side has disconnected. better than a timeout
    CLIENT_SERVER_NUM_PACKETS
};

struct ConnectionDisconnectPacket : public Packet
{
    uint64_t client_salt;
    uint64_t challenge_salt;

    ConnectionDisconnectPacket() : Packet(PACKET_CONNECTION_DISCONNECT)
    {
        client_salt = 0;
        challenge_salt = 0;
    }

    template <typename Stream> bool Serialize(Stream& stream)
    {
        serialize_uint64(stream, client_salt);
        serialize_uint64(stream, challenge_salt);
        return true;
    }

    PROTOCOL2_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
};

struct ConnectionRequestPacket : public Packet
{
    uint64_t client_salt;
    uint8_t data[256];

    ConnectionRequestPacket() : Packet(PACKET_CONNECTION_REQUEST)
    {
        client_salt = 0;
        memset(data, 0, sizeof(data));
    }

    template <typename Stream> bool Serialize(Stream& stream)
    {
        serialize_uint64(stream, client_salt);
        if (Stream::IsReading && stream.GetBitsRemaining() < 256 * 8)
            return false;
        serialize_bytes(stream, data, 256);
        return true;
    }

    PROTOCOL2_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
};

struct ConnectionKeepAlivePacket : public Packet
{
    uint64_t client_salt;
    uint64_t challenge_salt;

    ConnectionKeepAlivePacket() : Packet(PACKET_CONNECTION_KEEP_ALIVE)
    {
        client_salt = 0;
        challenge_salt = 0;
    }

    template <typename Stream> bool Serialize(Stream& stream)
    {
        serialize_uint64(stream, client_salt);
        serialize_uint64(stream, challenge_salt);
        return true;
    }

    PROTOCOL2_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
};

enum ClientState
{
    CLIENT_STATE_DISCONNECTED,
    CLIENT_STATE_SENDING_CONNECTION_REQUEST,
    CLIENT_STATE_SENDING_CHALLENGE_RESPONSE,
    CLIENT_STATE_CONNECTED,
    CLIENT_STATE_CONNECTION_REQUEST_TIMED_OUT,
    CLIENT_STATE_CHALLENGE_RESPONSE_TIMED_OUT,
    CLIENT_STATE_KEEP_ALIVE_TIMED_OUT,
    CLIENT_STATE_CONNECTION_DENIED_FULL,
    CLIENT_STATE_CONNECTION_DENIED_ALREADY_CONNECTED
};

class Client
{
    Socket* m_socket;                                                  // socket for sending and receiving packets.

    PacketFactory* m_packetFactory;                                    // packet factory for creating and deleting packets.

    ClientState m_clientState;                                          // current client state

    Address m_serverAddress;                                            // server address we are connecting or connected to.

    uint64_t m_clientSalt;                                              // client salt. randomly generated on each call to connect.

    uint64_t m_challengeSalt;                                           // challenge salt sent back from server in connection challenge.

    double m_lastPacketSendTime;                                        // time we last sent a packet to the server.

    double m_lastPacketReceiveTime;                                     // time we last received a packet from the server (used for timeouts).

    double m_clientSaltExpiryTime;                                      // time the client salt expires and we roll another (in case of collision).

public:

    Client(Socket& socket, PacketFactory& packetFactory)
    {
        m_socket = &socket;
        m_packetFactory = &packetFactory;
        ResetConnectionData();
    }

    ~Client()
    {
        assert(m_socket);
        assert(m_packetFactory);
        m_packetFactory = NULL;
        m_socket = NULL;
    }

    void Connect(const Address& address, double time)
    {
        Disconnect(time);
        m_challengeSalt = 0;
        m_serverAddress = address;
        m_clientState = CLIENT_STATE_SENDING_CONNECTION_REQUEST;
        m_lastPacketSendTime = time - 1.0f;
        m_lastPacketReceiveTime = time;
        m_clientSaltExpiryTime = time + ClientSaltTimeout;
    }

    bool IsConnecting() const
    {
        return m_clientState == CLIENT_STATE_SENDING_CONNECTION_REQUEST || m_clientState == CLIENT_STATE_SENDING_CHALLENGE_RESPONSE;
    }

    bool IsConnected() const
    {
        return m_clientState == CLIENT_STATE_CONNECTED;
    }

    bool ConnectionFailed() const
    {
        return m_clientState > CLIENT_STATE_CONNECTED;
    }

    void Disconnect(double time)
    {
        if (m_clientState == CLIENT_STATE_CONNECTED)
        {
            printf("client-side disconnect: (client salt = % " PRIx64 ", challenge salt = %" PRIx64 ")\n", m_clientSalt, m_challengeSalt);
            ConnectionDisconnectPacket* packet = (ConnectionDisconnectPacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_DISCONNECT);

            packet->client_salt = m_clientSalt;
            packet->challenge_salt = m_challengeSalt;
            SendPacketToServer(packet, time);
        }

        ResetConnectionData();
    }

    void SendPackets(double time)
    {
        switch (m_clientState)
        {
        case CLIENT_STATE_SENDING_CONNECTION_REQUEST:
        {
            if (m_lastPacketSendTime + ConnectionRequestSendRate > time)
                return;

            char buffer[256];
            const char* addressString = m_serverAddress.ToString(buffer, sizeof(buffer));
            printf("client sending connection request to server: %s\n", addressString);

            ConnectionRequestPacket* packet = (ConnectionRequestPacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_REQUEST);
            SendPacketToServer(packet, time);
        }
        break;

        case CLIENT_STATE_CONNECTED:
        {
            if (m_lastPacketSendTime + ConnectionKeepAliveSendRate > time)
                return;

            ConnectionKeepAlivePacket* packet = (ConnectionKeepAlivePacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);
            packet->client_salt = m_clientSalt;
            packet->challenge_salt = m_challengeSalt;

            SendPacketToServer(packet, time);
        }
        break;

        default:
            break;
        }
    }

    void ReceivePackets(double time)
    {
        while (true)
        {
            Address address;
            Packet * packet = ReceivePacket(m_socket, m_packetFactory, address);
            if (!packet)
                break;

            switch (packet->GetType())
            {

            default:
                break;
            }

            m_packetFactory->DestroyPacket(packet);
        }
    }

    void CheckForTimeOut(double time)
    {
        switch (m_clientState)
        {
        case CLIENT_STATE_SENDING_CONNECTION_REQUEST:
        {
            if (m_lastPacketReceiveTime < time)
            {
                printf("connection request to server timed out\n");
                m_clientState = CLIENT_STATE_CONNECTION_REQUEST_TIMED_OUT;
                return;
            }

            if (m_clientSaltExpiryTime < time)
            {
                //printf("client salt timed out. new client salt is %" PRIx64 "\n", m_clientSalt);
            }
        }
        break;

        case CLIENT_STATE_SENDING_CHALLENGE_RESPONSE:
        {
            if (m_lastPacketReceiveTime < time)
            {
                printf("challenge response to server timed out\n");
                m_clientState = CLIENT_STATE_CHALLENGE_RESPONSE_TIMED_OUT;
                return;
            }
        }
        break;

        case CLIENT_STATE_CONNECTED:
        {
            if (m_lastPacketReceiveTime < time)
            {
                printf("keep alive timed out\n");
                m_clientState = CLIENT_STATE_KEEP_ALIVE_TIMED_OUT;
                Disconnect(time);
                return;
            }
        }
        break;

        default:
            break;
        }
    }

protected:

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

    Packet* ReceivePacket(Socket* socket, PacketFactory* packetFactory, Address& address)
    {
        uint8_t packetData[MaxPacketSize];

        int packetBytes = 0;
         //   socket->ReceivePacket(address, packetData, MaxPacketSize);

        if (!packetBytes)
            return NULL;

        protocol2::PacketInfo info;
        info.protocolId = ProtocolId;
        info.packetFactory = packetFactory;

        return protocol2::ReadPacket(info, packetData, packetBytes, NULL, NULL);
    }

    void ResetConnectionData()
    {
        m_serverAddress = Address();
        m_clientState = CLIENT_STATE_DISCONNECTED;
        m_clientSalt = 0;
        m_challengeSalt = 0;
        m_lastPacketSendTime = -1000.0;
        m_lastPacketReceiveTime = -1000.0;
    }

    void SendPacketToServer(Packet* packet, double time)
    {
        assert(m_clientState != CLIENT_STATE_DISCONNECTED);
        assert(m_serverAddress.IsValid());

        SendPacket(m_socket, m_packetFactory, m_serverAddress, packet);

        m_lastPacketSendTime = time;
    }
};

#endif // !CLIENT_H

//#define WIN32_LEAN_AND_MEAN
//
//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
//
//
//#pragma comment (lib, "Ws2_32.lib")
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27016"
//class Client
//{
//public:
//	Client();
//	~Client();
//	void sendMsg();
//	bool OnCreate();
//private:
//
//};