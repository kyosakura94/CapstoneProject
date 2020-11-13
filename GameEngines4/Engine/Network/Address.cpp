#include "Address.h"
#include <WinSock2.h>
#include <cassert>

namespace network1 
{
    static bool s_networkInitialized = false;

    bool InitializeNetwork()
    {
        assert(!s_networkInitialized);

        bool result = true;

        WSADATA WsaData;
        result = WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;

        if (result)
            s_networkInitialized = result;

        return result;
    }

    void ShutdownNetwork()
    {
        assert(s_networkInitialized);
        WSACleanup();
        s_networkInitialized = false;
    }

    bool IsNetworkInitialized()
    {
        return s_networkInitialized;
    }


    Address::Address()
    {
    }

    Address::~Address()
    {
    }

    Address::Address(uint32_t address, int16_t port)
    {
        m_type = ADDRESS_IPV4;
        m_address_ipv4 = htonl(address);
        m_port = port;
    }

    Address::Address(const sockaddr_storage& addr_)
    {
    }

    Address::Address(const uint16_t address[], uint16_t port)
    {
    }

    Address::Address(const char* address, uint16_t port)
    {
        Parse(address);
        m_port = port;
    }
    //Address::Address(const sockaddr_storage& addr)
    //{
    //    if (addr.ss_family == AF_INET)
    //    {
    //        const sockaddr_in& addr_ipv4 = reinterpret_cast<const sockaddr_in&>(addr);
    //        m_type = ADDRESS_IPV4;
    //        m_address_ipv4 = addr_ipv4.sin_addr.s_addr;
    //        m_port = ntohs(addr_ipv4.sin_port);
    //    }
    //}

    bool Address::IsValid() const
    {
        return m_type != ADDRESS_UNDEFINED;
    }

    uint32_t Address::GetAddress4() const
    {
        assert(m_type == ADDRESS_IPV4);
        return m_address_ipv4;
    }

    uint16_t Address::GetPort() const
    {
        return m_port;
    }

    void Address::Clear()
    {
        m_type = ADDRESS_UNDEFINED;
        memset(m_address_ipv6, 0, sizeof(m_address_ipv6));
        m_port = 0;
    }

    void Address::Parse(const char* address_in)
    {
        // first try to parse as an IPv6 address:
        // 1. if the first character is '[' then it's probably an ipv6 in form "[addr6]:portnum"
        // 2. otherwise try to parse as raw IPv6 address, parse using inet_pton

        assert(address_in);

        char buffer[256];

        char* address = buffer;
        strcpy_s(address, 255, address_in);

        address[255] = '\0';

        int addressLength = (int)strlen(address);
        m_port = 0;
        if (address[0] == '[')
        {
            const int base_index = addressLength - 1;
            for (int i = 0; i < 6; ++i)   // note: no need to search past 6 characters as ":65535" is longest port value
            {
                const int index = base_index - i;
                if (index < 3)
                    break;
                if (address[index] == ':')
                {
                    m_port = uint16_t(atoi(&address[index + 1]));
                    address[index - 1] = '\0';
                }
            }
            address += 1;
        }


        // otherwise it's probably an IPv4 address:
        // 1. look for ":portnum", if found save the portnum and strip it out
        // 2. parse remaining ipv4 address via inet_pton

        addressLength = (int)strlen(address);
        const int base_index = addressLength - 1;
        for (int i = 0; i < 6; ++i)   // note: no need to search past 6 characters as ":65535" is longest port value
        {
            const int index = base_index - i;
            if (index < 0)
                break;
            if (address[index] == ':')
            {
                m_port = (uint16_t)atoi(&address[index + 1]);
                address[index] = '\0';
            }
        }

        struct sockaddr_in sockaddr4;
        if (inet_pton(AF_INET, address, &sockaddr4.sin_addr) == 1)
        {
            m_type = ADDRESS_IPV4;
            m_address_ipv4 = sockaddr4.sin_addr.s_addr;
        }
        else
        {
            // nope: it's not an IPv4 address. maybe it's a hostname? set address as undefined.
            Clear();
        }
    }


    const char* Address::ToString(char buffer[], int bufferSize) const
    {
        if (m_type == ADDRESS_IPV4)
        {
            const uint8_t a = m_address_ipv4 & 0xff;
            const uint8_t b = (m_address_ipv4 >> 8) & 0xff;
            const uint8_t c = (m_address_ipv4 >> 16) & 0xff;
            const uint8_t d = (m_address_ipv4 >> 24) & 0xff;
            if (m_port != 0)
                snprintf(buffer, bufferSize, "%d.%d.%d.%d:%d", a, b, c, d, m_port);
            else
                snprintf(buffer, bufferSize, "%d.%d.%d.%d", a, b, c, d);
            return buffer;
        }
        else
        {
            return "undefined";
        }
    }

    bool Address::operator ==(const Address& other) const
    {
        if (m_type != other.m_type)
            return false;
        if (m_port != other.m_port)
            return false;
        if (m_type == ADDRESS_IPV4 && m_address_ipv4 == other.m_address_ipv4)
            return true;
        else if (m_type == ADDRESS_IPV6 && memcmp(m_address_ipv6, other.m_address_ipv6, sizeof(m_address_ipv6)) == 0)
            return true;
        else
            return false;
    }
    bool Address::operator !=(const Address& other) const
    {
        return !(*this == other);
    }


}


