#include "Engine/Core/CoreEngine.h"
#include "Game/GameTest.h"
#include "Engine/Network/Client.h"
#include "Engine/Network/Server.h"

using namespace network1;

int main(int argc, char* argv[]) 
{
	printf("\nclient server\n\n");

	InitializeNetwork();

	Address clientAddress("::1", ClientPort);
	Address serverAddress("::1", ServerPort);

	Socket clientSocket(ClientPort);
	Socket serverSocket(ServerPort);

	if (clientSocket.GetError() != SOCKET_ERROR_NONE || serverSocket.GetError() != SOCKET_ERROR_NONE)
	{
		printf("error: failed to initialize sockets\n");
		return 1;
	}

	CoreEngine::GetInstance()->SetGameInterface(new GameTest);
	if (!CoreEngine::GetInstance()->OnCreate("GameEngine", 800, 600))
	{
		return 0;
	}

	CoreEngine::GetInstance()->Run();

	ShutdownNetwork();
	printf("\n");


	return 0;
}