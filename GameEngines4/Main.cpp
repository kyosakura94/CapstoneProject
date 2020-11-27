#include "Engine/Core/CoreEngine.h"
#include "Game/GameTest.h"
#include "Engine/Network/Client.h"
#include "Engine/Network/Server.h"
#include <thread>

using namespace network1;
using namespace protocol2;

void task1(string msg)
{
	printf("\server\n\n");

	InitializeNetwork();

	Address serverAddress("127.0.0.1", ServerPort);
	Socket serverSocket(ServerPort);

	if (serverSocket.GetError() != SOCKET_ERROR_NONE)
	{
		printf("error: failed to initialize sockets\n");
	}

	ClientServerPacketFactory serverPackage;
	Server server(serverSocket, serverPackage);
	Server::getInstance()->setServerSocket(serverSocket, serverPackage);

	bool isRunning = true;
	Timer timer;

	while (isRunning)
	{
		printf("\Accept packets\n\n");
		timer.UpdateFrameTicks();
		Server::getInstance()->ReceivePackets(timer.GetDeltaTime());
		Server::getInstance()->CheckForTimeOut(timer.GetDeltaTime());
	}


	ShutdownNetwork();
	printf("\n");
}

int main(int argc, char* argv[]) 
{

	InitializeNetworkClient(false);
	Address serverAddress("127.0.0.1", ServerPort);

	Socket clientSocket(ServerPort);

	if (clientSocket.GetError() != SOCKET_ERROR_NONE)
	{
		printf("error: failed to initialize sockets\n");
		return 1;
	}

	ClientServerPacketFactory clientPackage;

	Client client(clientSocket, clientPackage);
	Client::getInstance()->setClientSocket(clientSocket, clientPackage);
	
	Client::getInstance()->Connect(serverAddress, 0.0f);


	//for (int i = 0; i < NumIterations; ++i)
	//{
	//	printf("t = %f\n", time);

	//	Client::getInstance()->SendPackets(time);

	//	Server::getInstance()->ReceivePackets(time);
	//	Server::getInstance()->SendPackets(time);

	//	Client::getInstance()->ReceivePackets(time);

	//	Client::getInstance()->CheckForTimeOut(time);

	//	Server::getInstance()->CheckForTimeOut(time);

	//	if (Client::getInstance()->ConnectionFailed())
	//	{
	//		printf("error: client connect failed!\n");
	//		break;
	//	}

	//	if (Client::getInstance()->IsConnected())
	//		break;

	//	time += 0.1f;

	//	printf("----------------------------------------------------------\n");

	//	if (!Client::getInstance()->IsConnecting() && !Client::getInstance()->IsConnected() && server.GetNumConnectedClients() == 0)
	//		break;
	//}

	//while (true)
	//{
	//	printf("t = %f\n", time);




		//Server::getInstance()->ReceivePackets(time);
		//Server::getInstance()->SendPackets(time);
		//Server::getInstance()->CheckForTimeOut(time);

	//	if (Client::getInstance()->ConnectionFailed())
	//	{
	//		printf("error: client connect failed!\n");
	//		break;
	//	}

	//	if (Client::getInstance()->IsConnected())
	//		break;

	//	time += 0.1f;

	//	printf("----------------------------------------------------------\n");

	//	if (!Client::getInstance()->IsConnecting() && !Client::getInstance()->IsConnected() && server.GetNumConnectedClients() == 0)
	//		break;

	//}




	//for (int i = 0; i < NumIterations; ++i)
	//{
	//	printf("t = %f\n", time);

	//	server.SendPackets(time);

	//	server.ReceivePackets(time);

	//	server.CheckForTimeOut(time);

	//	if (client.ConnectionFailed())
	//	{
	//		printf("error: client connect failed!\n");
	//		break;
	//	}

	//	if (client.IsConnected())
	//		client.Disconnect(time);

	//	time += 0.1f;

	//	printf("----------------------------------------------------------\n");

	//	if (!client.IsConnecting() && !client.IsConnected() && server.GetNumConnectedClients() == 0)
	//		break;
	//}

	//This is client side, when it connected to server
	//its will create a scene, this will be the same to all other client


	CoreEngine::GetInstance()->SetGameInterface(new GameTest);
	if (!CoreEngine::GetInstance()->OnCreate("GameEngine", 800, 600))
	{
		return 0;
	}

	//Core game loop
	CoreEngine::GetInstance()->Run();

	thread t1(task1, "Hello");

	ShutdownNetwork();
	printf("\n");


	return 0;
}

