#include "Socket.h"

using namespace network1;

Socket::Socket(uint16_t port, SocketType type)
{
    m_error = SOCKET_ERROR_NONE;

    // create socket

    //m_socket = socket((type == SOCKET_TYPE_IPV6) ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    m_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (m_socket <= 0)
    {
        m_error = SOCKET_ERROR_CREATE_FAILED;
        return;
    }

    // bind to port

    sockaddr_in sock_address;
    sock_address.sin_family = AF_INET;
    sock_address.sin_addr.s_addr = INADDR_ANY;
    sock_address.sin_port = htons(port);

    if (::bind(m_socket, (const sockaddr*)&sock_address, sizeof(sock_address)) < 0)
    {
        m_error = SOCKET_ERROR_BIND_IPV4_FAILED;
        return;
    }

    // if bound to port 0 find the actual port we got

    m_port = port;

    if (m_port == 0)
    {
        struct sockaddr_in sin;
        socklen_t len = sizeof(sin);
        if (getsockname(m_socket, (struct sockaddr*) & sin, &len) == -1)
        {
            m_error = SOCKET_ERROR_GET_SOCKNAME_IPV4_FAILED;
            return;
        }
        m_port = ntohs(sin.sin_port);
    }

}

Socket::~Socket()
{

}

bool Socket::IsError() const
{
	return m_error != SOCKET_ERROR_NONE;;
}

int Socket::GetError() const
{
	return m_error;
}

bool Socket::SendPacket(const Address& address, const void* packetData, size_t packetBytes)
{
    assert(packetData);
    assert(packetBytes > 0);
    assert(address.IsValid());
    assert(m_socket);
    assert(!IsError());

    bool result = false;

    sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = address.GetAddress4();
    socket_address.sin_port = htons((unsigned short)address.GetPort());

    size_t sent_bytes = sendto(m_socket, (const char*)packetData, (int)packetBytes, 0, (sockaddr*)&socket_address, sizeof(sockaddr_in));
    result = sent_bytes == packetBytes;

    return result;
}

int Socket::ReceivePacket(Address& from, void* packetData, int maxPacketSize)
{
    assert(m_socket);
    assert(packetData);
    assert(maxPacketSize > 0);

#if NETWORK2_PLATFORM == NETWORK2_PLATFORM_WINDOWS
    typedef int socklen_t;
#endif

    sockaddr_storage sockaddr_from;
    socklen_t fromLength = sizeof(sockaddr_from);

    int result = recvfrom(m_socket, (char*)packetData, maxPacketSize, 0, (sockaddr*)&sockaddr_from, &fromLength);

#if NETWORK2_PLATFORM == NETWORK2_PLATFORM_WINDOWS
    if (result == SOCKET_ERROR)
    {
        int error = WSAGetLastError();

        if (error == WSAEWOULDBLOCK)
            return 0;

        printf("recvfrom failed: %d\n", error);

        return 0;
    }
#else // #if NETWORK2_PLATFORM == NETWORK2_PLATFORM_WINDOWS
    if (result <= 0)
    {
        if (errno == EAGAIN)
            return 0;

        printf("recvfrom failed: %s\n", strerror(errno));

        return 0;
    }
#endif // #if NETWORK2_PLATFORM == NETWORK2_PLATFORM_WINDOWS

    from = Address(sockaddr_from);

    assert(result >= 0);

    const int bytesRead = result;

    return bytesRead;
}
