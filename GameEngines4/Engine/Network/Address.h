#ifndef ADDRESS_H
#define ADDRESS_H

#include <cstdint>
#include <stdio.h>
#include <WinSock2.h>
#include <ws2def.h>
#include <WS2tcpip.h>
#include "Socket.h"
#include "protocol2.h"

namespace network1
{
    const uint32_t ProtocolId = 0x12341651;
    const int MaxPacketSize = 1200;
    const int MaxClients = 32;
    const int ServerPort = 50000;
    const int ClientPort = 60000;
    const int ChallengeHashSize = 1024;
    const float ChallengeSendRate = 0.1f;
    const float ChallengeTimeOut = 10.0f;
    const float ConnectionRequestSendRate = 0.1f;
    const float ConnectionChallengeSendRate = 0.1f;
    const float ConnectionResponseSendRate = 0.1f;
    const float ConnectionConfirmSendRate = 0.1f;
    const float ConnectionKeepAliveSendRate = 1.0f;
    const float ConnectionRequestTimeOut = 5.0f;
    const float ChallengeResponseTimeOut = 5.0f;
    const float KeepAliveTimeOut = 10.0f;
    const float ClientSaltTimeout = 1.0f;

    bool InitializeNetwork();

    void ShutdownNetwork();

    bool IsNetworkInitialized();

    enum AddressType
    {
        ADDRESS_UNDEFINED,
        ADDRESS_IPV4,
        ADDRESS_IPV6
    };

    class Address
    {
        AddressType m_type;

        union
        {
            uint32_t m_address_ipv4;
            uint16_t m_address_ipv6[8];
        };

        uint16_t m_port;

    public:
        Address();
        ~Address();
        explicit Address(uint32_t address, int16_t port = 0);
        explicit Address(const sockaddr_storage& addr_);
        explicit Address(const uint16_t address[], uint16_t port = 0);
        explicit Address(const char* address, uint16_t port);
        bool IsValid() const;
        void Clear();
        uint32_t GetAddress4() const;
        uint16_t GetPort() const;
        const char* ToString(char buffer[], int bufferSize) const;
        bool operator ==(const Address& other) const;

        bool operator !=(const Address& other) const;
    protected:

        void Parse(const char* address);
    };

    enum SocketType
    {
        SOCKET_TYPE_IPV4,
        SOCKET_TYPE_IPV6
    };

    enum SocketError
    {
        SOCKET_ERROR_NONE,
        SOCKET_ERROR_CREATE_FAILED,
        SOCKET_ERROR_SET_NON_BLOCKING_FAILED,
        SOCKET_ERROR_SOCKOPT_IPV6_ONLY_FAILED,
        SOCKET_ERROR_BIND_IPV4_FAILED,
        SOCKET_ERROR_BIND_IPV6_FAILED,
        SOCKET_ERROR_GET_SOCKNAME_IPV4_FAILED,
        SOCKET_ERROR_GET_SOCKNAME_IPV6_FAILED
    };

    typedef uint64_t SocketHandle;

    class Socket
    {
    public:

        Socket(uint16_t port, SocketType type = SOCKET_TYPE_IPV4);

        ~Socket();

        bool IsError() const;

        int GetError() const;

        bool SendPacket(const Address& address, const void* packetData, size_t packetBytes);

        int ReceivePacket(Address& from, void* packetData, int maxPacketSize);

    private:

        int m_error;
        uint16_t m_port;
        SocketHandle m_socket;
    };
}

#endif // !ADDRESS_H



