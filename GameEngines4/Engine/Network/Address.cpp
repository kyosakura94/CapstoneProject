#include "Address.h"
#include <WinSock2.h>
#include <cassert>

using namespace protocol2;
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
    
    bool InitializeNetworkClient(bool s_networkInitialized_)
    {
        assert(!s_networkInitialized_);

        bool result = true;

        WSADATA WsaData;
        result = WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;

        if (result)
            s_networkInitialized_ = result;

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

    Address::Address(const uint16_t address[], uint16_t port)
    {
    }

    Address::Address(const char* address, uint16_t port)
    {
        Parse(address);
        m_port = port;
    }

    Address::Address(const sockaddr_storage& addr)
    {
        if (addr.ss_family == AF_INET)
        {
            const sockaddr_in& addr_ipv4 = reinterpret_cast<const sockaddr_in&>(addr);
            m_type = ADDRESS_IPV4;
            m_address_ipv4 = addr_ipv4.sin_addr.s_addr;
            m_port = ntohs(addr_ipv4.sin_port);
        }
    }

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

protocol2::PacketFactory::PacketFactory(int numPacketTypes)
{
    m_numPacketTypes = numPacketTypes;
    m_numAllocatedPackets = 0;
}

protocol2::PacketFactory::~PacketFactory()
{

#if PROTOCOL2_DEBUG_MEMORY_LEAKS
    if (allocated_packets.size())
    {
        printf("you leaked packets!\n");
        printf("%d packets leaked\n", m_numAllocatedPackets);
        for (auto itor : allocated_packets)
        {
            auto p = itor.first;
            printf("leaked packet %p\n", p);
        }
    }
#endif // #if PROTOCOL2_DEBUG_MEMORY_LEAKS

    assert(m_numAllocatedPackets == 0);
}

Packet* protocol2::PacketFactory::CreatePacket(int type)
{
    assert(type >= 0);
    assert(type < m_numPacketTypes);

    Packet* packet = Create(type);
    if (!packet)
        return NULL;

#if PROTOCOL2_DEBUG_MEMORY_LEAKS
    printf("create packet %p\n", packet);
    allocated_packets[packet] = 1;
    auto itor = allocated_packets.find(packet);
    assert(itor != allocated_packets.end());
#endif // #if PROTOCOL2_DEBUG_MEMORY_LEAKS

    m_numAllocatedPackets++;

    return packet;
}


void protocol2::PacketFactory::DestroyPacket(Packet* packet)
{
    if (!packet)
        return;

#if PROTOCOL2_DEBUG_MEMORY_LEAKS
    printf("destroy packet %p\n", packet);
    auto itor = allocated_packets.find(packet);
    assert(itor != allocated_packets.end());
    allocated_packets.erase(packet);
#endif // #if PROTOCOL2_DEBUG_MEMORY_LEAKS

    assert(m_numAllocatedPackets > 0);

    m_numAllocatedPackets--;

    Destroy(packet);
}

int protocol2::PacketFactory::GetNumPacketTypes() const
{
    return m_numPacketTypes;
}

static const uint32_t crc32_table[256] =
{
    0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,
    0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,
    0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,
    0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
    0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,
    0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,
    0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
    0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,
    0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,
    0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,
    0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,
    0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,
    0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,
    0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
    0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,
    0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,
    0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,
    0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
    0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,
    0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,
    0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
    0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,
    0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,
    0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,
    0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,
    0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
    0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,
    0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
    0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,
    0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,
    0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,
    0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D
};

uint32_t protocol2::calculate_crc32(const uint8_t* buffer, size_t length, uint32_t crc32)
{
    crc32 ^= 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i)
        crc32 = (crc32 >> 8) ^ crc32_table[(crc32 ^ buffer[i]) & 0xFF];
    return crc32 ^ 0xFFFFFFFF;
}

uint32_t protocol2::hash_data(const uint8_t* data, uint32_t length, uint32_t hash)
{
    assert(data);
    for (uint32_t i = 0; i < length; ++i)
    {
        hash += data[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    return hash;
}

uint32_t protocol2::hash_string(const char string[], uint32_t hash)
{
    assert(string);
    while (*string != '\0')
    {
        char c = *string++;
        if ((c >= 'a') && (c <= 'z'))
            c = (c - 'a') + 'A';
        hash += c;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    return hash;
}

uint64_t protocol2::murmur_hash_64(const void* key, uint32_t length, uint64_t seed)
{
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const uint32_t r = 47;

    uint64_t h = seed ^ (length * m);

    const uint64_t* data = (const uint64_t*)key;
    const uint64_t* end = data + length / 8;

    while (data != end)
    {
#if PROTOCOL2_LITTLE_ENDIAN
        uint64_t k = *data++;
#else // #if PROTOCOL2_LITTLE_ENDIAN
        uint64_t k = *data++;
        uint8_t* p = (uint8_t*)&k;
        uint8_t c;
        c = p[0]; p[0] = p[7]; p[7] = c;
        c = p[1]; p[1] = p[6]; p[6] = c;
        c = p[2]; p[2] = p[5]; p[5] = c;
        c = p[3]; p[3] = p[4]; p[4] = c;
#endif // #if PROTOCOL2_LITTLE_ENDIAN

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const uint8_t* data2 = (const uint8_t*)data;

    switch (length & 7)
    {
    case 7: h ^= uint64_t(data2[6]) << 48;
    case 6: h ^= uint64_t(data2[5]) << 40;
    case 5: h ^= uint64_t(data2[4]) << 32;
    case 4: h ^= uint64_t(data2[3]) << 24;
    case 3: h ^= uint64_t(data2[2]) << 16;
    case 2: h ^= uint64_t(data2[1]) << 8;
    case 1: h ^= uint64_t(data2[0]);
        h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

int protocol2::WritePacket(const PacketInfo& info, Packet* packet, uint8_t* buffer, int bufferSize, Object* header)
{
    assert(packet);
    assert(buffer);
    assert(bufferSize > 0);
    assert(info.protocolId);
    assert(info.packetFactory);

    const int numPacketTypes = info.packetFactory->GetNumPacketTypes();

    WriteStream stream(buffer, bufferSize);

    stream.SetContext(info.context);

    for (int i = 0; i < info.prefixBytes; ++i)
    {
        uint8_t zero = 0;
        stream.SerializeBits(zero, 8);
    }

    uint32_t crc32 = 0;

    if (!info.rawFormat)
        stream.SerializeBits(crc32, 32);

    if (header)
    {
        if (!header->SerializeInternal(stream))
            return 0;
    }

    int packetType = packet->GetType();

    assert(numPacketTypes > 0);

    if (numPacketTypes > 1)
    {
        stream.SerializeInteger(packetType, 0, numPacketTypes - 1);
    }

    if (!packet->SerializeInternal(stream))
        return 0;

    stream.SerializeCheck("end of packet");

    stream.Flush();

    if (!info.rawFormat)
    {
        uint32_t network_protocolId = host_to_network(info.protocolId);
        crc32 = calculate_crc32((uint8_t*)&network_protocolId, 4);
        crc32 = calculate_crc32(buffer + info.prefixBytes, stream.GetBytesProcessed() - info.prefixBytes, crc32);
        *((uint32_t*)(buffer + info.prefixBytes)) = host_to_network(crc32);
    }

    if (stream.GetError())
        return 0;

    return stream.GetBytesProcessed();
}

Packet* protocol2::ReadPacket(const PacketInfo& info, const uint8_t* buffer, int bufferSize, Object* header, int* errorCode)
{
    assert(buffer);
    assert(bufferSize > 0);
    assert(info.protocolId != 0);
    assert(info.packetFactory);

    if (errorCode)
        *errorCode = PROTOCOL2_ERROR_NONE;

    ReadStream stream(buffer, bufferSize);

    stream.SetContext(info.context);

    for (int i = 0; i < info.prefixBytes; ++i)
    {
        uint32_t dummy = 0;
        stream.SerializeBits(dummy, 8);
    }

    uint32_t read_crc32 = 0;
    if (!info.rawFormat)
    {
        stream.SerializeBits(read_crc32, 32);

        uint32_t network_protocolId = host_to_network(info.protocolId);
        uint32_t crc32 = calculate_crc32((const uint8_t*)&network_protocolId, 4);
        uint32_t zero = 0;
        crc32 = calculate_crc32((const uint8_t*)&zero, 4, crc32);
        crc32 = calculate_crc32(buffer + info.prefixBytes + 4, bufferSize - 4 - info.prefixBytes, crc32);

        if (crc32 != read_crc32)
        {
            printf("corrupt packet. expected crc32 %x, got %x\n", crc32, read_crc32);

            if (errorCode)
                *errorCode = PROTOCOL2_ERROR_CRC32_MISMATCH;
            return NULL;
        }
    }

    if (header)
    {
        if (!header->SerializeInternal(stream))
        {
            if (errorCode)
                *errorCode = PROTOCOL2_ERROR_SERIALIZE_HEADER_FAILED;
            return NULL;
        }
    }

    int packetType = 0;

    const int numPacketTypes = info.packetFactory->GetNumPacketTypes();

    assert(numPacketTypes > 0);

    if (numPacketTypes > 1)
    {
        if (!stream.SerializeInteger(packetType, 0, numPacketTypes - 1))
        {
            if (errorCode)
                *errorCode = PROTOCOL2_ERROR_INVALID_PACKET_TYPE;
            return NULL;
        }
    }

    if (info.allowedPacketTypes)
    {
        if (!info.allowedPacketTypes[packetType])
        {
            if (errorCode)
                *errorCode = PROTOCOL2_ERROR_PACKET_TYPE_NOT_ALLOWED;
            return NULL;
        }
    }

    protocol2::Packet* packet = info.packetFactory->CreatePacket(packetType);
    if (!packet)
    {
        if (errorCode)
            *errorCode = PROTOCOL2_ERROR_CREATE_PACKET_FAILED;
        return NULL;
    }

    if (!packet->SerializeInternal(stream))
    {
        if (errorCode)
            *errorCode = PROTOCOL2_ERROR_SERIALIZE_PACKET_FAILED;
        goto cleanup;
    }

#if PROTOCOL2_SERIALIZE_CHECKS
    if (!stream.SerializeCheck("end of packet"))
    {
        if (errorCode)
            *errorCode = PROTOCOL2_ERROR_SERIALIZE_CHECK_FAILED;
        goto cleanup;
    }
#endif // #if PROTOCOL2_SERIALIZE_CHECKS

    if (stream.GetError())
    {
        if (errorCode)
            *errorCode = stream.GetError();
        goto cleanup;
    }

    return packet;

cleanup:
    info.packetFactory->DestroyPacket(packet);
    return NULL;
}

const char* protocol2::GetErrorString(int error)
{
    switch (error)
    {
    case PROTOCOL2_ERROR_NONE:                          return "no error";
    case PROTOCOL2_ERROR_CRC32_MISMATCH:                return "crc32 mismatch";
    case PROTOCOL2_ERROR_INVALID_PACKET_TYPE:           return "invalid packet type";
    case PROTOCOL2_ERROR_CREATE_PACKET_FAILED:          return "create packet failed";
    case PROTOCOL2_ERROR_SERIALIZE_HEADER_FAILED:       return "serialize header failed";
    case PROTOCOL2_ERROR_SERIALIZE_PACKET_FAILED:       return "serialize packet failed";
    case PROTOCOL2_ERROR_SERIALIZE_CHECK_FAILED:        return "serialize check failed";
    case PROTOCOL2_ERROR_STREAM_OVERFLOW:               return "stream overflow";
    case PROTOCOL2_ERROR_STREAM_ABORTED:                return "stream aborted";

    default:
        return "???";
    }
}
