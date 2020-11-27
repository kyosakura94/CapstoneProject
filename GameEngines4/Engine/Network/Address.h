#ifndef ADDRESS_H
#define ADDRESS_H

#include <cstdint>
#include <stdio.h>
#include <WinSock2.h>
#include <ws2def.h>
#include <WS2tcpip.h>

namespace network1
{
    const uint32_t ProtocolId = 0x12341651;
    const int MaxPacketSize = 1200;
    const int MaxClients = 10;
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
    bool InitializeNetworkClient(bool check_);

    void ShutdownNetwork();
    void ShutdownNetworkClient();

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

        int getSocket() { return m_socket; }
    private:

        int m_error;
        uint16_t m_port;
        SocketHandle m_socket;
    };
}

#endif // !ADDRESS_H

#ifndef PROTOCOL2_H
#define PROTOCOL2_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#define PROTOCOL2_SERIALIZE_CHECKS              1
#define PROTOCOL2_DEBUG_PACKET_LEAKS            0
#define PROTOCOL2_PACKET_AGGREGATION            1

#if PROTOCOL2_DEBUG_PACKET_LEAKS
#include <stdio.h>
#include <map>
#endif // #if PROTOCOL2_DEBUG_PACKET_LEAKS

#if    defined(__386__) || defined(i386)    || defined(__i386__)  \
    || defined(__X86)   || defined(_M_IX86)                       \
    || defined(_M_X64)  || defined(__x86_64__)                    \
    || defined(alpha)   || defined(__alpha) || defined(__alpha__) \
    || defined(_M_ALPHA)                                          \
    || defined(ARM)     || defined(_ARM)    || defined(__arm__)   \
    || defined(WIN32)   || defined(_WIN32)  || defined(__WIN32__) \
    || defined(_WIN32_WCE) || defined(__NT__)                     \
    || defined(__MIPSEL__)
#define PROTOCOL2_LITTLE_ENDIAN 1
#else
#define PROTOCOL2_BIG_ENDIAN 1
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4127 )
#pragma warning( disable : 4244 )
#endif // #ifdef _MSC_VER

namespace protocol2
{
#ifndef min
    template <typename T> const T& min(const T& a, const T& b)
    {
        return (a < b) ? a : b;
    }
#endif

#ifndef max
    template <typename T> const T& max(const T& a, const T& b)
    {
        return (a > b) ? a : b;
    }
#endif

    template <typename T> T clamp(const T& value, const T& min, const T& max)
    {
        if (value < min)
            return min;
        else if (value > max)
            return max;
        else
            return value;
    }

    template <typename T> void swap(T& a, T& b)
    {
        T tmp = a;
        a = b;
        b = tmp;
    };

    template <typename T> T abs(const T& value)
    {
        return (value < 0) ? -value : value;
    }

    template <uint32_t x> struct PopCount
    {
        enum {
            a = x - ((x >> 1) & 0x55555555),
            b = (((a >> 2) & 0x33333333) + (a & 0x33333333)),
            c = (((b >> 4) + b) & 0x0f0f0f0f),
            d = c + (c >> 8),
            e = d + (d >> 16),

            result = e & 0x0000003f
        };
    };

    template <uint32_t x> struct Log2
    {
        enum {
            a = x | (x >> 1),
            b = a | (a >> 2),
            c = b | (b >> 4),
            d = c | (c >> 8),
            e = d | (d >> 16),
            f = e >> 1,

            result = PopCount<f>::result
        };
    };

    template <int64_t min, int64_t max> struct BitsRequired
    {
        static const uint32_t result = (min == max) ? 0 : (Log2<uint32_t(max - min)>::result + 1);
    };

#define BITS_REQUIRED( min, max ) BitsRequired<min,max>::result

    inline uint32_t popcount(uint32_t x)
    {
#ifdef __GNUC__
        return __builtin_popcount(x);
#else // #ifdef __GNUC__
        const uint32_t a = x - ((x >> 1) & 0x55555555);
        const uint32_t b = (((a >> 2) & 0x33333333) + (a & 0x33333333));
        const uint32_t c = (((b >> 4) + b) & 0x0f0f0f0f);
        const uint32_t d = c + (c >> 8);
        const uint32_t e = d + (d >> 16);
        const uint32_t result = e & 0x0000003f;
        return result;
#endif // #ifdef __GNUC__
    }

#ifdef __GNUC__

    inline int bits_required(uint32_t min, uint32_t max)
    {
        return 32 - __builtin_clz(max - min);
    }

#else // #ifdef __GNUC__

    inline uint32_t log2(uint32_t x)
    {
        const uint32_t a = x | (x >> 1);
        const uint32_t b = a | (a >> 2);
        const uint32_t c = b | (b >> 4);
        const uint32_t d = c | (c >> 8);
        const uint32_t e = d | (d >> 16);
        const uint32_t f = e >> 1;
        return popcount(f);
    }

    inline int bits_required(uint32_t min, uint32_t max)
    {
        return (min == max) ? 0 : log2(max - min) + 1;
    }

#endif // #ifdef __GNUC__

    inline uint32_t bswap(uint32_t value)
    {
#ifdef __GNUC__
        return __builtin_bswap32(value);
#else // #ifdef __GNUC__
        return (value & 0x000000ff) << 24 | (value & 0x0000ff00) << 8 | (value & 0x00ff0000) >> 8 | (value & 0xff000000) >> 24;
#endif // #ifdef __GNUC__
    }

    inline uint16_t bswap(uint16_t value)
    {
        return (value & 0x00ff) << 8 | (value & 0xff00) >> 8;
    }

    // IMPORTANT: These functions consider network order to be little endian because most modern processors are little endian. Least amount of work!

    inline uint32_t host_to_network(uint32_t value)
    {
        return value;
    }

    inline uint32_t network_to_host(uint32_t value)
    {
#if PROTOCOL2_BIG_ENDIAN
        return bswap(value);
#else // #if PROTOCOL2_BIG_ENDIAN
        return value;
#endif // #if PROTOCOL2_BIG_ENDIAN
    }

    inline uint16_t host_to_network(uint16_t value)
    {
#if PROTOCOL2_BIG_ENDIAN
        return bswap(value);
#else // #if PROTOCOL2_BIG_ENDIAN
        return value;
#endif // #if PROTOCOL2_BIG_ENDIAN
    }

    inline uint16_t network_to_host(uint16_t value)
    {
#if PROTOCOL2_BIG_ENDIAN
        return bswap(value);
#else // #if PROTOCOL2_BIG_ENDIAN
        return value;
#endif // #if PROTOCOL2_BIG_ENDIAN
    }

    inline bool sequence_greater_than(uint16_t s1, uint16_t s2)
    {
        return ((s1 > s2) && (s1 - s2 <= 32768)) ||
            ((s1 < s2) && (s2 - s1 > 32768));
    }

    inline bool sequence_less_than(uint16_t s1, uint16_t s2)
    {
        return sequence_greater_than(s2, s1);
    }

    inline int sequence_difference(uint16_t _s1, uint16_t _s2)
    {
        int s1 = _s1;
        int s2 = _s2;
        if (abs(s1 - s2) >= 32786)
        {
            if (s1 > s2)
                s2 += 65536;
            else
                s1 += 65536;
        }
        return s1 - s2;
    }

    inline int signed_to_unsigned(int n)
    {
        return (n << 1) ^ (n >> 31);
    }

    inline int unsigned_to_signed(uint32_t n)
    {
        return (n >> 1) ^ (-int32_t(n & 1));
    }

    uint32_t calculate_crc32(const uint8_t* buffer, size_t length, uint32_t crc32 = 0);

    uint32_t hash_data(const uint8_t* data, uint32_t length, uint32_t hash);

    uint32_t hash_string(const char string[], uint32_t hash);

    uint64_t murmur_hash_64(const void* key, uint32_t length, uint64_t seed);

    class BitWriter
    {
    public:

        BitWriter(void* data, int bytes)
            : m_data((uint32_t*)data), m_numWords(bytes / 4)
        {
            assert(data);
            assert((bytes % 4) == 0);           // buffer size must be a multiple of four
            m_numBits = m_numWords * 32;
            m_bitsWritten = 0;
            m_wordIndex = 0;
            m_scratch = 0;
            m_scratchBits = 0;
        }

        void WriteBits(uint32_t value, int bits)
        {
            assert(bits > 0);
            assert(bits <= 32);
            assert(m_bitsWritten + bits <= m_numBits);

            value &= (uint64_t(1) << bits) - 1;

            m_scratch |= uint64_t(value) << m_scratchBits;

            m_scratchBits += bits;

            if (m_scratchBits >= 32)
            {
                assert(m_wordIndex < m_numWords);
                m_data[m_wordIndex] = host_to_network(uint32_t(m_scratch & 0xFFFFFFFF));
                m_scratch >>= 32;
                m_scratchBits -= 32;
                m_wordIndex++;
            }

            m_bitsWritten += bits;
        }

        void WriteAlign()
        {
            const int remainderBits = m_bitsWritten % 8;
            if (remainderBits != 0)
            {
                uint32_t zero = 0;
                WriteBits(zero, 8 - remainderBits);
                assert((m_bitsWritten % 8) == 0);
            }
        }

        void WriteBytes(const uint8_t* data, int bytes)
        {
            assert(GetAlignBits() == 0);
            assert(m_bitsWritten + bytes * 8 <= m_numBits);
            assert((m_bitsWritten % 32) == 0 || (m_bitsWritten % 32) == 8 || (m_bitsWritten % 32) == 16 || (m_bitsWritten % 32) == 24);

            int headBytes = (4 - (m_bitsWritten % 32) / 8) % 4;
            if (headBytes > bytes)
                headBytes = bytes;
            for (int i = 0; i < headBytes; ++i)
                WriteBits(data[i], 8);
            if (headBytes == bytes)
                return;

            FlushBits();

            assert(GetAlignBits() == 0);

            int numWords = (bytes - headBytes) / 4;
            if (numWords > 0)
            {
                assert((m_bitsWritten % 32) == 0);
                memcpy(&m_data[m_wordIndex], data + headBytes, numWords * 4);
                m_bitsWritten += numWords * 32;
                m_wordIndex += numWords;
                m_scratch = 0;
            }

            assert(GetAlignBits() == 0);

            int tailStart = headBytes + numWords * 4;
            int tailBytes = bytes - tailStart;
            assert(tailBytes >= 0 && tailBytes < 4);
            for (int i = 0; i < tailBytes; ++i)
                WriteBits(data[tailStart + i], 8);

            assert(GetAlignBits() == 0);

            assert(headBytes + numWords * 4 + tailBytes == bytes);
        }

        void FlushBits()
        {
            if (m_scratchBits != 0)
            {
                assert(m_wordIndex < m_numWords);
                m_data[m_wordIndex] = host_to_network(uint32_t(m_scratch & 0xFFFFFFFF));
                m_scratch >>= 32;
                m_scratchBits -= 32;
                m_wordIndex++;
            }
        }

        int GetAlignBits() const
        {
            return (8 - (m_bitsWritten % 8)) % 8;
        }

        int GetBitsWritten() const
        {
            return m_bitsWritten;
        }

        int GetBitsAvailable() const
        {
            return m_numBits - m_bitsWritten;
        }

        const uint8_t* GetData() const
        {
            return (uint8_t*)m_data;
        }

        int GetBytesWritten() const
        {
            return (m_bitsWritten + 7) / 8;
        }

        int GetTotalBytes() const
        {
            return m_numWords * 4;
        }

    private:

        uint32_t* m_data;
        uint64_t m_scratch;
        int m_numBits;
        int m_numWords;
        int m_bitsWritten;
        int m_wordIndex;
        int m_scratchBits;
    };

    class BitReader
    {
    public:

#ifdef DEBUG
        BitReader(const void* data, int bytes) : m_data((const uint32_t*)data), m_numBytes(bytes)
#else // #ifdef DEBUG
        BitReader(const void* data, int bytes) : m_data((const uint32_t*)data), m_numBytes(bytes), m_numWords((bytes + 3) / 4)
#endif // #ifdef DEBUG
        {
            // IMPORTANT: Although we support non-multiples of four bytes passed in, the actual buffer
            // underneath the bit reader must round up to at least 4 bytes because we read a dword at a time.
            assert(data);
            m_numBits = m_numBytes * 8;
            m_bitsRead = 0;
            m_scratch = 0;
            m_scratchBits = 0;
            m_wordIndex = 0;
        }

        bool WouldOverflow(int bits) const
        {
            return m_bitsRead + bits > m_numBits;
        }

        uint32_t ReadBits(int bits)
        {
            assert(bits > 0);
            assert(bits <= 32);
            assert(m_bitsRead + bits <= m_numBits);

            m_bitsRead += bits;

            assert(m_scratchBits >= 0 && m_scratchBits <= 64);

            if (m_scratchBits < bits)
            {
                assert(m_wordIndex < m_numWords);
                m_scratch |= uint64_t(network_to_host(m_data[m_wordIndex])) << m_scratchBits;
                m_scratchBits += 32;
                m_wordIndex++;
            }

            assert(m_scratchBits >= bits);

            const uint32_t output = m_scratch & ((uint64_t(1) << bits) - 1);

            m_scratch >>= bits;
            m_scratchBits -= bits;

            return output;
        }

        bool ReadAlign()
        {
            const int remainderBits = m_bitsRead % 8;
            if (remainderBits != 0)
            {
                uint32_t value = ReadBits(8 - remainderBits);
                assert(m_bitsRead % 8 == 0);
                if (value != 0)
                    return false;
            }
            return true;
        }

        void ReadBytes(uint8_t* data, int bytes)
        {
            assert(GetAlignBits() == 0);
            assert(m_bitsRead + bytes * 8 <= m_numBits);
            assert((m_bitsRead % 32) == 0 || (m_bitsRead % 32) == 8 || (m_bitsRead % 32) == 16 || (m_bitsRead % 32) == 24);

            int headBytes = (4 - (m_bitsRead % 32) / 8) % 4;
            if (headBytes > bytes)
                headBytes = bytes;
            for (int i = 0; i < headBytes; ++i)
                data[i] = (uint8_t)ReadBits(8);
            if (headBytes == bytes)
                return;

            assert(GetAlignBits() == 0);

            int numWords = (bytes - headBytes) / 4;
            if (numWords > 0)
            {
                assert((m_bitsRead % 32) == 0);
                memcpy(data + headBytes, &m_data[m_wordIndex], numWords * 4);
                m_bitsRead += numWords * 32;
                m_wordIndex += numWords;
                m_scratchBits = 0;
            }

            assert(GetAlignBits() == 0);

            int tailStart = headBytes + numWords * 4;
            int tailBytes = bytes - tailStart;
            assert(tailBytes >= 0 && tailBytes < 4);
            for (int i = 0; i < tailBytes; ++i)
                data[tailStart + i] = (uint8_t)ReadBits(8);

            assert(GetAlignBits() == 0);

            assert(headBytes + numWords * 4 + tailBytes == bytes);
        }

        int GetAlignBits() const
        {
            return (8 - m_bitsRead % 8) % 8;
        }

        int GetBitsRead() const
        {
            return m_bitsRead;
        }

        int GetBytesRead() const
        {
            return m_wordIndex * 4;
        }

        int GetBitsRemaining() const
        {
            return m_numBits - m_bitsRead;
        }

        int GetBytesRemaining() const
        {
            return GetBitsRemaining() / 8;
        }

        int GetTotalBits() const
        {
            return m_numBits;
        }

        int GetTotalBytes() const
        {
            return m_numBits / 8;
        }

    private:

        const uint32_t* m_data;
        uint64_t m_scratch;
        int m_numBits;
        int m_numBytes;
        int m_numWords;
#ifdef DEBUG

#endif // #ifdef DEBUG
        int m_bitsRead;
        int m_scratchBits;
        int m_wordIndex;
    };

#define PROTOCOL2_ERROR_NONE                        0
#define PROTOCOL2_ERROR_CRC32_MISMATCH              1
#define PROTOCOL2_ERROR_INVALID_PACKET_TYPE         2
#define PROTOCOL2_ERROR_PACKET_TYPE_NOT_ALLOWED     3
#define PROTOCOL2_ERROR_CREATE_PACKET_FAILED        4
#define PROTOCOL2_ERROR_SERIALIZE_HEADER_FAILED     5
#define PROTOCOL2_ERROR_SERIALIZE_PACKET_FAILED     6
#define PROTOCOL2_ERROR_SERIALIZE_CHECK_FAILED      7
#define PROTOCOL2_ERROR_STREAM_OVERFLOW             8
#define PROTOCOL2_ERROR_STREAM_ABORTED              9

    class WriteStream
    {
    public:

        enum { IsWriting = 1 };
        enum { IsReading = 0 };

        WriteStream(uint8_t* buffer, int bytes) : m_error(PROTOCOL2_ERROR_NONE), m_context(NULL), m_writer(buffer, bytes) {}

        bool SerializeInteger(int32_t value, int32_t min, int32_t max)
        {
            assert(min < max);
            assert(value >= min);
            assert(value <= max);
            const int bits = bits_required(min, max);
            uint32_t unsigned_value = value - min;
            m_writer.WriteBits(unsigned_value, bits);
            return true;
        }

        bool SerializeBits(uint32_t value, int bits)
        {
            assert(bits > 0);
            assert(bits <= 32);
            m_writer.WriteBits(value, bits);
            return true;
        }

        bool SerializeBytes(const uint8_t* data, int bytes)
        {
            assert(data);
            assert(bytes >= 0);
            if (!SerializeAlign())
                return false;
            m_writer.WriteBytes(data, bytes);
            return true;
        }

        bool SerializeAlign()
        {
            m_writer.WriteAlign();
            return true;
        }

        int GetAlignBits() const
        {
            return m_writer.GetAlignBits();
        }

        bool SerializeCheck(const char* string)
        {
#if PROTOCOL2_SERIALIZE_CHECKS
            SerializeAlign();
            const uint32_t magic = hash_string(string, 0);
            SerializeBits(magic, 32);
#endif // #if PROTOCOL2_SERIALIZE_CHECKS
            return true;
        }

        void Flush()
        {
            m_writer.FlushBits();
        }

        const uint8_t* GetData() const
        {
            return m_writer.GetData();
        }

        int GetBytesProcessed() const
        {
            return m_writer.GetBytesWritten();
        }

        int GetBitsProcessed() const
        {
            return m_writer.GetBitsWritten();
        }

        int GetBitsRemaining() const
        {
            return GetTotalBits() - GetBitsProcessed();
        }

        int GetTotalBits() const
        {
            return m_writer.GetTotalBytes() * 8;
        }

        int GetTotalBytes() const
        {
            return m_writer.GetTotalBytes();
        }

        void SetContext(void* context)
        {
            m_context = context;
        }

        void* GetContext() const
        {
            return m_context;
        }

        int GetError() const
        {
            return m_error;
        }

    private:

        int m_error;
        void* m_context;
        BitWriter m_writer;
    };

    class ReadStream
    {
    public:

        enum { IsWriting = 0 };
        enum { IsReading = 1 };

        ReadStream(const uint8_t* buffer, int bytes) : m_context(NULL), m_error(PROTOCOL2_ERROR_NONE), m_bitsRead(0), m_reader(buffer, bytes) {}

        bool SerializeInteger(int32_t& value, int32_t min, int32_t max)
        {
            assert(min < max);
            const int bits = bits_required(min, max);
            if (m_reader.WouldOverflow(bits))
            {
                m_error = PROTOCOL2_ERROR_STREAM_OVERFLOW;
                return false;
            }
            uint32_t unsigned_value = m_reader.ReadBits(bits);
            value = (int32_t)unsigned_value + min;
            m_bitsRead += bits;
            return true;
        }

        bool SerializeBits(uint32_t& value, int bits)
        {
            assert(bits > 0);
            assert(bits <= 32);
            if (m_reader.WouldOverflow(bits))
            {
                m_error = PROTOCOL2_ERROR_STREAM_OVERFLOW;
                return false;
            }
            uint32_t read_value = m_reader.ReadBits(bits);
            value = read_value;
            m_bitsRead += bits;
            return true;
        }

        bool SerializeBytes(uint8_t* data, int bytes)
        {
            if (!SerializeAlign())
                return false;
            if (m_reader.WouldOverflow(bytes * 8))
            {
                m_error = PROTOCOL2_ERROR_STREAM_OVERFLOW;
                return false;
            }
            m_reader.ReadBytes(data, bytes);
            m_bitsRead += bytes * 8;
            return true;
        }

        bool SerializeAlign()
        {
            const int alignBits = m_reader.GetAlignBits();
            if (m_reader.WouldOverflow(alignBits))
            {
                m_error = PROTOCOL2_ERROR_STREAM_OVERFLOW;
                return false;
            }
            if (!m_reader.ReadAlign())
                return false;
            m_bitsRead += alignBits;
            return true;
        }

        int GetAlignBits() const
        {
            return m_reader.GetAlignBits();
        }

        bool SerializeCheck(const char* string)
        {
#if PROTOCOL2_SERIALIZE_CHECKS            
            SerializeAlign();
            uint32_t value = 0;
            SerializeAlign();
            SerializeBits(value, 32);
            const uint32_t magic = hash_string(string, 0);
            if (magic != value)
            {
                printf("serialize check failed: '%s'. expected %x, got %x\n", string, magic, value);
            }
            return value == magic;
#else // #if PROTOCOL2_SERIALIZE_CHECKS
            return true;
#endif // #if PROTOCOL2_SERIALIZE_CHECKS
        }

        int GetBitsProcessed() const
        {
            return m_bitsRead;
        }

        int GetBitsRemaining() const
        {
            return m_reader.GetBitsRemaining();
        }

        int GetBytesProcessed() const
        {
            return (m_bitsRead + 7) / 8;
        }

        void SetContext(void* context)
        {
            m_context = context;
        }

        void* GetContext() const
        {
            return m_context;
        }

        int GetError() const
        {
            return m_error;
        }

        int GetBytesRead() const
        {
            return m_reader.GetBytesRead();
        }

    private:

        void* m_context;
        int m_error;
        int m_bitsRead;
        BitReader m_reader;
    };

    class MeasureStream
    {
    public:

        enum { IsWriting = 1 };
        enum { IsReading = 0 };

        MeasureStream(int bytes) : m_context(NULL), m_error(PROTOCOL2_ERROR_NONE), m_totalBytes(bytes), m_bitsWritten(0) {}

#ifdef DEBUG
        bool SerializeInteger(int32_t value, int32_t min, int32_t max)
#else // #ifdef DEBUG
        bool SerializeInteger(int32_t value, int32_t min, int32_t max)
#endif // #ifdef DEBUG
        {
            assert(min < max);
            assert(value >= min);
            assert(value <= max);
            const int bits = bits_required(min, max);
            m_bitsWritten += bits;
            return true;
        }

        bool SerializeBits(uint32_t /*value*/, int bits)
        {
            assert(bits > 0);
            assert(bits <= 32);
            m_bitsWritten += bits;
            return true;
        }

        bool SerializeBytes(const uint8_t* /*data*/, int bytes)
        {
            SerializeAlign();
            m_bitsWritten += bytes * 8;
            return true;
        }

        bool SerializeAlign()
        {
            const int alignBits = GetAlignBits();
            m_bitsWritten += alignBits;
            return true;
        }

        int GetAlignBits() const
        {
            return 7;       // we can't know for sure, so be conservative and assume worst case
        }

        bool SerializeCheck(const char* /*string*/)
        {
#if PROTOCOL2_SERIALIZE_CHECKS
            SerializeAlign();
            m_bitsWritten += 32;
#endif // #if PROTOCOL2_SERIALIZE_CHECKS
            return true;
        }

        int GetBitsProcessed() const
        {
            return m_bitsWritten;
        }

        int GetBitsRemaining() const
        {
            return m_totalBytes * 8 - GetBitsProcessed();
        }

        int GetBytesProcessed() const
        {
            return (m_bitsWritten + 7) / 8;
        }

        int GetTotalBytes() const
        {
            return m_totalBytes;
        }

        int GetTotalBits() const
        {
            return m_totalBytes * 8;
        }

        void SetContext(void* context)
        {
            m_context = context;
        }

        void* GetContext() const
        {
            return m_context;
        }

        int GetError() const
        {
            return m_error;
        }

    private:

        void* m_context;
        int m_error;
        int m_totalBytes;
        int m_bitsWritten;
    };

#define serialize_int( stream, value, min, max )                    \
        do                                                              \
        {                                                               \
            assert( min < max );                                        \
            int32_t int32_value;                                        \
            if ( Stream::IsWriting )                                    \
            {                                                           \
                assert( int64_t(value) >= int64_t(min) );               \
                assert( int64_t(value) <= int64_t(max) );               \
                int32_value = (int32_t) value;                          \
            }                                                           \
            if ( !stream.SerializeInteger( int32_value, min, max ) )    \
                return false;                                           \
            if ( Stream::IsReading )                                    \
            {                                                           \
                value = int32_value;                                    \
                if ( value < min || value > max )                       \
                    return false;                                       \
            }                                                           \
        } while (0)

#define serialize_bits( stream, value, bits )						\
        do                                                              \
        {                                                               \
            assert( bits > 0 );                                         \
            assert( bits <= 32 );                                       \
            uint32_t uint32_value;                                      \
            if ( Stream::IsWriting )                                    \
                uint32_value = (uint32_t) value;                        \
            if ( !stream.SerializeBits( uint32_value, bits ) )          \
                return false;                                           \
            if ( Stream::IsReading )                                    \
                value = uint32_value;                                   \
        } while (0)

#define serialize_bool( stream, value )								\
		do																\
		{																\
            uint32_t uint32_bool_value;									\
			if ( Stream::IsWriting )									\
				uint32_bool_value = value ? 1 : 0; 						\
			serialize_bits( stream, uint32_bool_value, 1 );				\
			if ( Stream::IsReading )									\
				value = uint32_bool_value ? true : false;				\
		} while (0)

#define serialize_enum( stream, value, type, num_entries )          \
        do                                                              \
        {                                                               \
            uint32_t int_value;                                         \
            if ( Stream::IsWriting )                                    \
                int_value = value;                                      \
            serialize_int( stream, int_value, 0, num_entries - 1 );     \
            if ( Stream::IsReading )                                    \
                value = (type) value;                                   \
        } while (0) 

    template <typename Stream> bool serialize_float_internal(Stream& stream, float& value)
    {
        uint32_t int_value;

        if (Stream::IsWriting)
            memcpy(&int_value, &value, 4);

        bool result = stream.SerializeBits(int_value, 32);

        if (Stream::IsReading)
            memcpy(&value, &int_value, 4);

        return result;
    }

#define serialize_float( stream, value )                                        \
        do                                                                          \
        {                                                                           \
            if ( !protocol2::serialize_float_internal( stream, value ) )            \
                return false;                                                       \
        } while (0)

#define serialize_uint32( stream, value )                                       \
        serialize_bits( stream, value, 32 );

    template <typename Stream> bool serialize_uint64_internal(Stream& stream, uint64_t& value)
    {
        uint32_t hi, lo;
        if (Stream::IsWriting)
        {
            lo = value & 0xFFFFFFFF;
            hi = value >> 32;
        }
        serialize_bits(stream, lo, 32);
        serialize_bits(stream, hi, 32);
        if (Stream::IsReading)
            value = (uint64_t(hi) << 32) | lo;
        return true;
    }

#define serialize_uint64( stream, value )                                       \
        do                                                                          \
        {                                                                           \
            if ( !protocol2::serialize_uint64_internal( stream, value ) )           \
                return false;                                                       \
        } while (0)

    template <typename Stream> bool serialize_double_internal(Stream& stream, double& value)
    {
        union DoubleInt
        {
            double double_value;
            uint64_t int_value;
        };

        DoubleInt tmp;
        if (Stream::IsWriting)
            tmp.double_value = value;

        serialize_uint64(stream, tmp.int_value);

        if (Stream::IsReading)
            value = tmp.double_value;

        return true;
    }

#define serialize_double( stream, value )                                       \
        do                                                                          \
        {                                                                           \
            if ( !protocol2::serialize_double_internal( stream, value ) )           \
                return false;                                                       \
        } while (0)

    template <typename Stream> bool serialize_bytes_internal(Stream& stream, uint8_t* data, int bytes)
    {
        return stream.SerializeBytes(data, bytes);
    }

#define serialize_bytes( stream, data, bytes )                                  \
        do                                                                          \
        {                                                                           \
            if ( !protocol2::serialize_bytes_internal( stream, data, bytes ) )      \
                return false;                                                       \
        } while (0)

    template <typename Stream> bool serialize_string_internal(Stream& stream, char* string, int buffer_size)
    {
        int length;
        if (Stream::IsWriting)
        {
            length = (int)strlen(string);
            assert(length < buffer_size - 1);
        }
        serialize_int(stream, length, 0, buffer_size - 1);
        serialize_bytes(stream, (uint8_t*)string, length);
        if (Stream::IsReading)
            string[length] = '\0';
        return true;
    }

#define serialize_string( stream, string, buffer_size )                                 \
        do                                                                                  \
        {                                                                                   \
            if ( !protocol2::serialize_string_internal( stream, string, buffer_size ) )     \
                return false;                                                               \
        } while (0)

#define serialize_align( stream )                                                       \
        do                                                                                  \
        {                                                                                   \
            if ( !stream.SerializeAlign() )                                                 \
                return false;                                                               \
        } while (0)

#define serialize_check( stream, string )                                               \
        do                                                                                  \
        {                                                                                   \
            if ( !stream.SerializeCheck( string ) )                                         \
                return false;                                                               \
        } while (0)

#define serialize_object( stream, object )                                              \
        do                                                                                  \
        {                                                                                   \
            if ( !object.Serialize( stream ) )                                              \
                return false;                                                               \
        }                                                                                   \
        while(0)

#define read_bits( stream, value, bits )                                                \
    do                                                                                      \
    {                                                                                       \
        assert( bits > 0 );                                                                 \
        assert( bits <= 32 );                                                               \
        uint32_t uint32_value;                                                              \
        if ( !stream.SerializeBits( uint32_value, bits ) )                                  \
            return false;                                                                   \
        value = uint32_value;                                                               \
    } while (0)

#define read_int( stream, value, min, max )                                             \
        do                                                                                  \
        {                                                                                   \
            assert( min < max );                                                            \
            int32_t int32_value;                                                            \
            if ( !stream.SerializeInteger( int32_value, min, max ) )                        \
                return false;                                                               \
            value = int32_value;                                                            \
            if ( value < min || value > max )                                               \
                return false;                                                               \
        } while (0)

#define read_bool( stream, value ) read_bits( stream, value, 1 )

#define read_float     serialize_float
#define read_uint32    serialize_uint32
#define read_uint64    serialize_uint64
#define read_double    serialize_double
#define read_bytes     serialize_bytes
#define read_string    serialize_string
#define read_align     serialize_align
#define read_check     serialize_check
#define read_object    serialize_object

#define write_bits( stream, value, bits )                                               \
        do                                                                                  \
        {                                                                                   \
            assert( bits > 0 );                                                             \
            assert( bits <= 32 );                                                           \
            uint32_t uint32_value = (uint32_t) value;                                       \
            if ( !stream.SerializeBits( uint32_value, bits ) )                              \
                return false;                                                               \
        } while (0)

#define write_int( stream, value, min, max )                                            \
        do                                                                                  \
        {                                                                                   \
            assert( min < max );                                                            \
            assert( value >= min );                                                         \
            assert( value <= max );                                                         \
            int32_t int32_value = (int32_t) value;                                          \
            if ( !stream.SerializeInteger( int32_value, min, max ) )                        \
                return false;                                                               \
        } while (0)

#define write_float    serialize_float
#define write_uint32   serialize_uint32
#define write_uint64   serialize_uint64
#define write_double   serialize_double
#define write_bytes    serialize_bytes
#define write_string   serialize_string
#define write_align    serialize_align
#define write_check    serialize_check
#define write_object   serialize_object

    class Object
    {
    public:

        virtual ~Object() {}

        virtual bool SerializeInternal(class ReadStream& stream) = 0;

        virtual bool SerializeInternal(class WriteStream& stream) = 0;

        virtual bool SerializeInternal(class MeasureStream& stream) = 0;
    };

#define PROTOCOL2_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS()                                                     \
        bool SerializeInternal( class protocol2::ReadStream & stream ) { return Serialize( stream ); };         \
        bool SerializeInternal( class protocol2::WriteStream & stream ) { return Serialize( stream ); };        \
        bool SerializeInternal( class protocol2::MeasureStream & stream ) { return Serialize( stream ); };      \

    class Packet : public Object
    {
        int type;

    public:

        Packet(int _type) : type(_type) {}

        virtual ~Packet() {}

        int GetType() const { return type; }

    private:

        Packet(const Packet& other);
        Packet& operator = (const Packet& other);
    };

    class PacketFactory
    {
        int m_numPacketTypes;
        int m_numAllocatedPackets;
#if PROTOCOL2_DEBUG_MEMORY_LEAKS
        std::map<void*, int> allocated_packets;
#endif // #if PROTOCOL2_DEBUG_MEMORY_LEAKS

    public:

        PacketFactory(int numTypes);

        ~PacketFactory();

        Packet* CreatePacket(int type);

        void DestroyPacket(Packet* packet);

        int GetNumPacketTypes() const;

    protected:

        virtual Packet* Create(int type) = 0;
        virtual void Destroy(Packet* packet) = 0;
    };

    struct PacketInfo
    {
        bool rawFormat;                             // if true packets are written in "raw" format without crc32 (useful for encrypted packets).
        int prefixBytes;                            // prefix this number of bytes when reading and writing packets. stick your own data there.
        uint32_t protocolId;                        // protocol id that distinguishes your protocol from other packets sent over UDP.
        PacketFactory* packetFactory;              // create packets and determine information about packet types. required.
        const uint8_t* allowedPacketTypes;         // array of allowed packet types. if a packet type is not allowed the serialize read or write will fail.
        void* context;                             // context for the packet serialization (optional, pass in NULL)

        PacketInfo()
        {
            rawFormat = false;
            prefixBytes = 0;
            protocolId = 0;
            packetFactory = NULL;
            allowedPacketTypes = NULL;
            context = NULL;
        }
    };

    int WritePacket(const PacketInfo& info,
        Packet* packet,
        uint8_t* buffer,
        int bufferSize,
        Object* header = NULL);

    Packet* ReadPacket(const PacketInfo& info,
        const uint8_t* buffer,
        int bufferSize,
        Object* header = NULL,
        int* errorCode = NULL);

#if PROTOCOL2_PACKET_AGGREGATION

    int WriteAggregatePacket(const PacketInfo& info,
        int numPackets,
        Packet** packets,
        uint8_t* buffer,
        int bufferSize,
        int& numPacketsWritten,
        Object* aggregatePacketHeader = NULL,
        Object** packetHeaders = NULL);

    void ReadAggregatePacket(const PacketInfo& info,
        int maxPacketsToRead,
        Packet** packets,
        const uint8_t* buffer,
        int bufferSize,
        int& numPacketsRead,
        Object* aggregatePacketHeader = NULL,
        Object** packetHeaders = NULL,
        int* errorCode = NULL);

#endif // #if PROTOCOL2_PACKET_AGGREGATION

    const char* GetErrorString(int error);

    class BitArray
    {
    public:

        BitArray(int size)
        {
            assert(size > 0);
            m_size = size;
            m_bytes = 8 * ((size / 64) + ((size % 64) ? 1 : 0));
            assert(m_bytes > 0);
            assert((m_bytes % 8) == 0);
            m_data = new uint64_t[m_bytes / 8];
            Clear();
        }

        ~BitArray()
        {
            assert(m_data);
            delete[] m_data;
            m_data = NULL;
        }

        void Clear()
        {
            assert(m_data);
            memset(m_data, 0, m_bytes);
        }

        void SetBit(int index)
        {
            assert(index >= 0);
            assert(index < m_size);
            const int data_index = index >> 6;
            const int bit_index = index & ((1 << 6) - 1);
            assert(bit_index >= 0);
            assert(bit_index < 64);
            m_data[data_index] |= uint64_t(1) << bit_index;
        }

        void ClearBit(int index)
        {
            assert(index >= 0);
            assert(index < m_size);
            const int data_index = index >> 6;
            const int bit_index = index & ((1 << 6) - 1);
            m_data[data_index] &= ~(uint64_t(1) << bit_index);
        }

        uint64_t GetBit(int index) const
        {
            assert(index >= 0);
            assert(index < m_size);
            const int data_index = index >> 6;
            const int bit_index = index & ((1 << 6) - 1);
            assert(bit_index >= 0);
            assert(bit_index < 64);
            return (m_data[data_index] >> bit_index) & 1;
        }

        int GetSize() const
        {
            return m_size;
        }

    private:

        int m_size;
        int m_bytes;
        uint64_t* m_data;

        BitArray(const BitArray& other);
        BitArray& operator = (const BitArray& other);
    };

    template <typename T> class SequenceBuffer
    {
    public:

        SequenceBuffer(int size) : m_exists(size)
        {
            assert(size > 0);
            m_size = size;
            m_first_entry = true;
            m_sequence = 0;
            m_entry_sequence = new uint16_t[size];
            m_entry_data = new T[size];
            Reset();
        }

        ~SequenceBuffer()
        {
            assert(m_entry_data);
            assert(m_entry_sequence);
            delete[] m_entry_data;
            delete[] m_entry_sequence;
            m_entry_data = NULL;
            m_entry_sequence = NULL;
        }

        void Reset()
        {
            m_first_entry = true;
            m_sequence = 0;
            m_exists.Clear();
            memset(m_entry_sequence, 0, sizeof(uint16_t) * m_size);
        }

        T* Insert(uint16_t sequence)
        {
            if (m_first_entry)
            {
                m_sequence = sequence + 1;
                m_first_entry = false;
            }
            else if (sequence_greater_than(sequence + 1, m_sequence))
            {
                m_sequence = sequence + 1;
            }
            else if (sequence_less_than(sequence, m_sequence - m_size))
            {
                return NULL;
            }

            const int index = sequence % m_size;

            m_exists.SetBit(index);

            m_entry_sequence[index] = sequence;

            return &m_entry_data[index];
        }

        void Remove(uint16_t sequence)
        {
            m_exists.ClearBit(sequence % m_size);
        }

        void RemoveOldEntries()
        {
            const uint16_t oldest_sequence = m_sequence - m_size;
            for (int i = 0; i < m_size; ++i)
            {
                if (m_exists.GetBit(i) && sequence_less_than(m_entry_sequence[i], oldest_sequence))
                    m_exists.ClearBit(i);
            }
        }

        bool IsAvailable(uint16_t sequence) const
        {
            return !m_exists.GetBit(sequence % m_size);
        }

        int GetIndex(uint16_t sequence) const
        {
            return sequence % m_size;
        }

        const T* Find(uint16_t sequence) const
        {
            const int index = sequence % m_size;
            if (m_exists.GetBit(index) && m_entry_sequence[index] == sequence)
                return &m_entry_data[index];
            else
                return NULL;
        }

        T* Find(uint16_t sequence)
        {
            const int index = sequence % m_size;
            if (m_exists.GetBit(index) && m_entry_sequence[index] == sequence)
                return &m_entry_data[index];
            else
                return NULL;
        }

        T* GetAtIndex(int index)
        {
            assert(index >= 0);
            assert(index < m_size);
            return m_exists.GetBit(index) ? &m_entry_data[index] : NULL;
        }

        uint16_t GetSequence() const
        {
            return m_sequence;
        }

        int GetSize() const
        {
            return m_size;
        }

    private:

        T* m_entry_data;
        uint16_t* m_entry_sequence;
        int m_size;
        uint16_t m_sequence;
        bool m_first_entry;
        BitArray m_exists;

        SequenceBuffer(const SequenceBuffer<T>& other);
        SequenceBuffer<T>& operator = (const SequenceBuffer<T>& other);
    };

    template <typename T> void GenerateAckBits(const SequenceBuffer<T>& packets, uint16_t& ack, uint32_t& ack_bits)
    {
        ack = packets.GetSequence() - 1;
        ack_bits = 0;
        for (int i = 0; i < 32; ++i)
        {
            uint16_t sequence = ack - i;
            if (packets.Find(sequence))
                ack_bits |= (1 << i);
        }
    }

    inline void CompressPacketSequence(uint64_t sequence, uint8_t& prefix_byte, int& num_sequence_bytes, uint8_t* sequence_bytes)
    {
        // algorithm: encode a mask of 7 bits. each bit is set if byte n in the sequence is non-zero

        assert(sequence_bytes);

        prefix_byte = 0;

        num_sequence_bytes = 0;

        uint64_t byte_mask = 0xFF00000000000000ULL;

        for (int i = 7; i > 0; --i)
        {
            const uint8_t current_sequence_byte = uint8_t((sequence & byte_mask) >> (i * 8));

            if (current_sequence_byte != 0)
            {
                sequence_bytes[num_sequence_bytes++] = current_sequence_byte;
                prefix_byte |= (1 << (i - 1));
            }

            byte_mask >>= 8;
        }

        assert(num_sequence_bytes <= 7);
        assert((prefix_byte & (1 << 7)) == 0);

        sequence_bytes[num_sequence_bytes++] = (uint8_t)(sequence & 0xFF);
    }

    inline int GetPacketSequenceBytes(uint8_t prefix_byte)
    {
        int num_sequence_bytes = 0;

        for (int i = 7; i > 0; --i)
        {
            if (prefix_byte & (1 << (i - 1)))
                num_sequence_bytes++;
        }

        num_sequence_bytes++;

        return num_sequence_bytes;
    }

    inline uint64_t DecompressPacketSequence(uint8_t prefix_byte, const uint8_t* sequence_bytes)
    {
        assert(sequence_bytes);

        uint64_t sequence = 0;

        int index = 0;

        for (int i = 7; i > 0; --i)
        {
            if (prefix_byte & (1 << (i - 1)))
            {
                sequence |= (uint64_t(sequence_bytes[index]) << (i * 8));
                ++index;
            }
        }

        sequence |= uint64_t(sequence_bytes[index]);

        return sequence;
    }
}

#endif

