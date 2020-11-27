#ifndef CLIENT_H
#define CLIENT_H

#include <cassert>
#include <inttypes.h>
#include "Address.h";
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "../Core/Timer.h"
#include <json.hpp>
#include <iostream>

using namespace network1;
using namespace protocol2;
using namespace glm;
using namespace std;
using json = nlohmann::json;
enum PacketTypes
{
	PACKET_CONNECTION_REQUEST,                      // client requests a connection.
	PACKET_CONNECTION_DENIED,                       // server denies client connection request.
	PACKET_CONNECTION_CHALLENGE,                    // server response to client connection request.
	PACKET_CONNECTION_RESPONSE,                     // client response to server connection challenge.
	PACKET_CONNECTION_KEEP_ALIVE,                   // keep alive packet sent at some low rate (once per-second) to keep the connection alive
	PACKET_CONNECTION_DISCONNECT,                   // courtesy packet to indicate that the other side has disconnected. better than a timeout
	TEST_PACKET_CREATEPLAYER,
	PACKET_JSON,
	PACKET_UPDATE_POSITION,
	CLIENT_SERVER_NUM_PACKETS
};

enum ConnectionDeniedReason
{
	CONNECTION_DENIED_SERVER_FULL,
	CONNECTION_DENIED_ALREADY_CONNECTED,
	CONNECTION_DENIED_NUM_VALUES
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

struct ConnectionDeniedPacket : public Packet
{
	uint64_t client_salt;
	ConnectionDeniedReason reason;

	ConnectionDeniedPacket() : Packet(PACKET_CONNECTION_DENIED)
	{
		client_salt = 0;
		reason = CONNECTION_DENIED_NUM_VALUES;
	}

	template <typename Stream> bool Serialize(Stream& stream)
	{
		serialize_uint64(stream, client_salt);
		serialize_enum(stream, reason, ConnectionDeniedReason, CONNECTION_DENIED_NUM_VALUES);
		return true;
	}

	PROTOCOL2_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();
};

struct ConnectionChallengePacket : public Packet
{
	uint64_t client_salt;
	uint64_t challenge_salt;

	ConnectionChallengePacket() : Packet(PACKET_CONNECTION_CHALLENGE)
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

struct ConnectionResponsePacket : public Packet
{
	uint64_t client_salt;
	uint64_t challenge_salt;

	ConnectionResponsePacket() : Packet(PACKET_CONNECTION_RESPONSE)
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



struct TestPacket : public Packet
{
	vec3 position;
	uint8_t  tagName[256];
	uint8_t  modelName[256];


	uint64_t client_salt;
	uint64_t challenge_salt;

	TestPacket() : Packet(TEST_PACKET_CREATEPLAYER)
	{
		client_salt = 0;
		challenge_salt = 0;
	}

	void setUpData(std::string modelName_, std::string tagName_, vec3 position_)
	{
		//std::string::size_type sz;

		//strcpy(modelName, modelName_.c_str());

		//memset(tagName, std::stoi(modelName_, &sz),256);
		//memset(modelName, std::stoi(tagName_, &sz),256);

		position = position_;

		//char* modelNametmp;
		//char* tagNametmp;
		//modelNametmp = new char[modelName_.length() + 1];
		//tagNametmp = new char[tagName_.length() + 1];

		//strcpy(modelNametmp, modelName_.c_str());
		//strcpy(tagNametmp, tagName_.c_str());

		//tagName = (uint8_t*)atoi(tagNametmp);
		//modelName = (uint8_t*)atoi(modelNametmp);


		strcpy((char*)tagName, tagName_.c_str());
		strcpy((char*)modelName, modelName_.c_str());

		//memset(tagName, 0, sizeof(tagName));
		//memset(modelName, 0, sizeof(modelName));

	}

	template <typename Stream> bool Serialize(Stream& stream)
	{
		serialize_float(stream, position.x);
		serialize_float(stream, position.y);
		serialize_float(stream, position.z);

		serialize_uint64(stream, client_salt);
		serialize_uint64(stream, challenge_salt);

		//serialize_string_internal(stream, modelName, 256);
		//serialize_string_internal(stream, tagName, 256);

		if (Stream::IsReading && stream.GetBitsRemaining() < 256 * 8)
			return false;

		serialize_bytes(stream, tagName, 256);
		serialize_bytes(stream, modelName, 256);


		return true;
	}

	PROTOCOL2_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();

	bool operator == (const TestPacket& other)
	{
		return position.x == other.position.x &&
			position.y == other.position.y &&
			position.z == other.position.z &&
			modelName == other.modelName &&
			tagName == other.tagName;
	}
};

struct JsonPacket : public Packet
{
	uint8_t  jsonString[256];

	JsonPacket() : Packet(PACKET_JSON)
	{
	}

	void setUpData(std::string jsonString_)
	{
		strcpy((char*)jsonString, jsonString_.c_str());
	}

	template <typename Stream> bool Serialize(Stream& stream)
	{

		if (Stream::IsReading && stream.GetBitsRemaining() < 256 * 8)
			return false;

		serialize_bytes(stream, jsonString, 256);

		return true;
	}

	PROTOCOL2_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();

	bool operator == (const JsonPacket& other)
	{
		return jsonString == other.jsonString;
	}

};

struct UpdatePosition : public Packet
{
	vec3 position;
	uint8_t  tagName[256];

	UpdatePosition() : Packet(PACKET_UPDATE_POSITION)
	{

	}

	void setUpData(std::string tagName_, vec3 position_)
	{
		position = position_;
		strcpy((char*)tagName, tagName_.c_str());
	}

	template <typename Stream> bool Serialize(Stream& stream)
	{
		serialize_float(stream, position.x);
		serialize_float(stream, position.y);
		serialize_float(stream, position.z);

		if (Stream::IsReading && stream.GetBitsRemaining() < 256 * 8)
			return false;

		serialize_bytes(stream, tagName, 256);
		return true;
	}

	PROTOCOL2_DECLARE_VIRTUAL_SERIALIZE_FUNCTIONS();

	bool operator == (const TestPacket& other)
	{
		return position.x == other.position.x &&
			position.y == other.position.y &&
			position.z == other.position.z &&
			tagName == other.tagName;
	}
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

struct ServerChallengeEntry
{
	uint64_t client_salt;                           // random number generated by client and sent to server in connection request
	uint64_t challenge_salt;                        // random number generated by server and sent back to client in challenge packet
	double create_time;                             // time this challenge entry was created. used for challenge timeout
	double last_packet_send_time;                   // the last time we sent a challenge packet to this client
	Address address;                                // address the connection request came from
};

struct ServerChallengeHash
{
	int num_entries;
	uint8_t exists[ChallengeHashSize];
	ServerChallengeEntry entries[ChallengeHashSize];

	ServerChallengeHash() { memset(this, 0, sizeof(ServerChallengeHash)); }
};

struct ClientServerPacketFactory : public PacketFactory
{
	ClientServerPacketFactory() : PacketFactory(CLIENT_SERVER_NUM_PACKETS) {}

	Packet* Create(int type)
	{
		switch (type)
		{
			case PACKET_CONNECTION_REQUEST:         return new ConnectionRequestPacket();
			case PACKET_CONNECTION_DENIED:          return new ConnectionDeniedPacket();
			case PACKET_CONNECTION_CHALLENGE:       return new ConnectionChallengePacket();
			case PACKET_CONNECTION_RESPONSE:        return new ConnectionResponsePacket();
			case PACKET_CONNECTION_KEEP_ALIVE:      return new ConnectionKeepAlivePacket();
			case PACKET_CONNECTION_DISCONNECT:      return new ConnectionDisconnectPacket();
			case TEST_PACKET_CREATEPLAYER:			return new TestPacket();
			case PACKET_UPDATE_POSITION:			return new UpdatePosition();
			case PACKET_JSON:						return new JsonPacket();
			default:
				return NULL;
		}
	}

	void Destroy(Packet* packet)
	{
		delete packet;
	}
};

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

	static std::unique_ptr<Client> ClientInstance;
	friend std::default_delete<Client>;

	Packet* receviedPacket;


public:


	Client(const Client&) = delete;
	Client(Client&&) = delete;
	Client& operator = (const Client&) = delete;
	Client& operator = (Client&&) = delete;
	static Client* getInstance();

	Client(Socket& socket, PacketFactory& packetFactory)
	{
		m_socket = &socket;
		m_packetFactory = &packetFactory;
		ResetConnectionData();
	}

	Packet* getRecievedPacket() { return receviedPacket; }

	Client();

	~Client()
	{
		assert(m_socket);
		assert(m_packetFactory);
		m_packetFactory = NULL;
		m_socket = NULL;
	}

	PacketFactory* getPacketFactory() { return m_packetFactory; };

	void setClientSocket(Socket& socket, PacketFactory& packetFactory)
	{
		m_socket = &socket;
		m_packetFactory = &packetFactory;
	}

	void Connect(const Address& address, double time)
	{
		Disconnect(time);
		m_clientSalt = GenerateSalt();
		m_challengeSalt = 0;
		m_serverAddress = address;
		m_clientState = CLIENT_STATE_SENDING_CONNECTION_REQUEST;
		m_lastPacketSendTime = time - 1.0f;
		m_lastPacketReceiveTime = time;
		m_clientSaltExpiryTime = time + ClientSaltTimeout;
	}

	uint64_t GenerateSalt()
	{
		return ((uint64_t(rand()) << 0) & 0x000000000000FFFFull) |
			((uint64_t(rand()) << 16) & 0x00000000FFFF0000ull) |
			((uint64_t(rand()) << 32) & 0x0000FFFF00000000ull) |
			((uint64_t(rand()) << 48) & 0xFFFF000000000000ull);
	};

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
			printf("client-side disconnect: (client salt = %" PRIx64 ", challenge salt = %" PRIx64 ")\n", m_clientSalt, m_challengeSalt);
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
			packet->client_salt = m_clientSalt;

			SendPacketToServer(packet, time);
		}
		break;

		case CLIENT_STATE_SENDING_CHALLENGE_RESPONSE:
		{
			if (m_lastPacketSendTime + ConnectionResponseSendRate > time)
				return;

			char buffer[256];
			const char* addressString = m_serverAddress.ToString(buffer, sizeof(buffer));
			printf("client sending challenge response to server: %s\n", addressString);

			ConnectionResponsePacket* packet = (ConnectionResponsePacket*)m_packetFactory->CreatePacket(PACKET_CONNECTION_RESPONSE);
			packet->client_salt = m_clientSalt;
			packet->challenge_salt = m_challengeSalt;

			SendPacketToServer(packet, time);
		}
		break;

		case CLIENT_STATE_CONNECTED:
		{
			//if (m_lastPacketSendTime + ConnectionKeepAliveSendRate > time)
			//	return;
			//printf("Keep Connected to Server \n");
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

	void SendPackets()
	{
		switch (m_clientState)
		{
		case CLIENT_STATE_CONNECTED:
		{
			//TestPacket* packet = (TestPacket*)m_packetFactory->CreatePacket(TEST_PACKET_CREATEPLAYER);
			//packet->setUpData("redDice", "Player1", vec3(2.0f, 0.0f, -5.0f));

			//SendPacketToServer(packet, Timer::Timer().GetDeltaTime());

			json j;

			j["modelName"] = "redDice";
			j["tag"] = "Player1";
			j["position"] = { 2, 0, -5 };

			std::string s = j.dump(); 

			std::cout << "Json Send: " << s << std::endl;
			JsonPacket* packet = (JsonPacket*)m_packetFactory->CreatePacket(PACKET_JSON);

			packet->setUpData(s);

			SendPacketToServer(packet, Timer::Timer().GetDeltaTime());
		}
	
		break;

		default:
			break;
		}
	}

	void SendPackets(Packet* packet)
	{
		switch (m_clientState)
		{
		case CLIENT_STATE_CONNECTED:
		{
			SendPacketToServer(packet, Timer::Timer().GetDeltaTime());
		}
		break;

		default:
			break;
		}
	}

	Packet* ReceiveAPacket(double time)
	{
		Address address;
		Packet* packet = ReceivePacket(m_socket, m_packetFactory, address);

		switch (packet->GetType())
		{
		case PACKET_CONNECTION_DENIED:
			ProcessConnectionDenied(*(ConnectionDeniedPacket*)packet, address, time);
			return packet;
			break;

		case PACKET_CONNECTION_CHALLENGE:
			ProcessConnectionChallenge(*(ConnectionChallengePacket*)packet, address, time);
			return packet;
			break;

		case PACKET_CONNECTION_KEEP_ALIVE:
			ProcessConnectionKeepAlive(*(ConnectionKeepAlivePacket*)packet, address, time);
			return packet;
			break;

		case PACKET_CONNECTION_DISCONNECT:
			ProcessConnectionDisconnect(*(ConnectionDisconnectPacket*)packet, address, time);
			return packet;
			break;

		case TEST_PACKET_CREATEPLAYER:
			return packet;
			break;
		case PACKET_JSON:
			return packet;
			break;

		default:
			return nullptr;
			break;
		}

	}

	void ReceivePackets(double time)
	{
		Address address;
		Packet* packet = ReceivePacket(m_socket, m_packetFactory, address);
		//if (!packet)
		//	break;

		switch (packet->GetType())
		{
		case PACKET_CONNECTION_DENIED:
			ProcessConnectionDenied(*(ConnectionDeniedPacket*)packet, address, time);
			break;

		case PACKET_CONNECTION_CHALLENGE:
			ProcessConnectionChallenge(*(ConnectionChallengePacket*)packet, address, time);
			break;

		case PACKET_CONNECTION_KEEP_ALIVE:
			ProcessConnectionKeepAlive(*(ConnectionKeepAlivePacket*)packet, address, time);
			break;

		case PACKET_CONNECTION_DISCONNECT:
			ProcessConnectionDisconnect(*(ConnectionDisconnectPacket*)packet, address, time);
			break;

		case TEST_PACKET_CREATEPLAYER:
			receviedPacket = packet;
			break;		
		
		case PACKET_UPDATE_POSITION:
			receviedPacket = packet;
			break;		
		
		case PACKET_JSON:
			
			receviedPacket = packet;
			break;

		default:
			break;
		}

		m_packetFactory->DestroyPacket(packet);
	}

	void CheckForTimeOut(double time)
	{
		switch (m_clientState)
		{
		case CLIENT_STATE_SENDING_CONNECTION_REQUEST:
		{
			if (m_lastPacketReceiveTime + ConnectionRequestTimeOut < time)
			{
				printf("connection request to server timed out\n");
				m_clientState = CLIENT_STATE_CONNECTION_REQUEST_TIMED_OUT;
				return;
			}

			if (m_clientSaltExpiryTime < time)
			{
				m_clientSalt = GenerateSalt();
				m_clientSaltExpiryTime = time + ClientSaltTimeout;
				printf("client salt timed out. new client salt is %" PRIx64 "\n", m_clientSalt);
			}
		}
		break;

		case CLIENT_STATE_SENDING_CHALLENGE_RESPONSE:
		{
			if (m_lastPacketReceiveTime + ChallengeResponseTimeOut < time)
			{
				printf("challenge response to server timed out\n");
				m_clientState = CLIENT_STATE_CHALLENGE_RESPONSE_TIMED_OUT;
				return;
			}
		}
		break;

		case CLIENT_STATE_CONNECTED:
		{
			if (m_lastPacketReceiveTime + KeepAliveTimeOut < time)
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

	void ProcessConnectionDenied(const ConnectionDeniedPacket& packet, const Address& address, double /*time*/)
	{
		if (m_clientState != CLIENT_STATE_SENDING_CONNECTION_REQUEST)
			return;

		if (packet.client_salt != m_clientSalt)
			return;

		if (address != m_serverAddress)
			return;

		char buffer[256];
		const char* addressString = address.ToString(buffer, sizeof(buffer));
		if (packet.reason == CONNECTION_DENIED_SERVER_FULL)
		{
			printf("client received connection denied from server: %s (server is full)\n", addressString);
			m_clientState = CLIENT_STATE_CONNECTION_DENIED_FULL;
		}
		else if (packet.reason == CONNECTION_DENIED_ALREADY_CONNECTED)
		{
			printf("client received connection denied from server: %s (already connected)\n", addressString);
			m_clientState = CLIENT_STATE_CONNECTION_DENIED_ALREADY_CONNECTED;
		}
	}

	void ProcessConnectionChallenge(const ConnectionChallengePacket& packet, const Address& address, double time)
	{
		if (m_clientState != CLIENT_STATE_SENDING_CONNECTION_REQUEST)
			return;

		if (packet.client_salt != m_clientSalt)
			return;

		if (address != m_serverAddress)
			return;

		char buffer[256];
		const char* addressString = address.ToString(buffer, sizeof(buffer));
		printf("client received connection challenge from server: %s (challenge salt = %" PRIx64 ")\n", addressString, packet.challenge_salt);

		m_challengeSalt = packet.challenge_salt;

		m_clientState = CLIENT_STATE_SENDING_CHALLENGE_RESPONSE;

		m_lastPacketReceiveTime = time;
	}

	void ProcessConnectionKeepAlive(const ConnectionKeepAlivePacket& packet, const Address& address, double time)
	{
		if (m_clientState < CLIENT_STATE_SENDING_CHALLENGE_RESPONSE)
			return;

		if (packet.client_salt != m_clientSalt)
			return;

		if (packet.challenge_salt != m_challengeSalt)
			return;

		if (address != m_serverAddress)
			return;

		if (m_clientState == CLIENT_STATE_SENDING_CHALLENGE_RESPONSE)
		{
			char buffer[256];
			const char* addressString = address.ToString(buffer, sizeof(buffer));
			printf("client is now connected to server: %s\n", addressString);
			m_clientState = CLIENT_STATE_CONNECTED;
		}

		m_lastPacketReceiveTime = time;
	}

	void ProcessConnectionDisconnect(const ConnectionDisconnectPacket& packet, const Address& address, double time)
	{
		if (m_clientState != CLIENT_STATE_CONNECTED)
			return;

		if (packet.client_salt != m_clientSalt)
			return;

		if (packet.challenge_salt != m_challengeSalt)
			return;

		if (address != m_serverAddress)
			return;

		Disconnect(time);
	}
};

#endif // !CLIENT_H
