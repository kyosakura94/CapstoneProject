//#include "Client.h"
//
////
////Client::Client()
////{
////}
////
////Client::~Client()
////{
////}
////
////void Client::sendMsg()
////{
////
////}
////
////bool Client::OnCreate()
//{
//	WSADATA wsaData;
//	SOCKET ConnectSocket = INVALID_SOCKET;
//	struct addrinfo* result = NULL,
//		* ptr = NULL,
//		hints;
//	char sendbuf[] = "this is a test";
//	char recvbuf[DEFAULT_BUFLEN];
//	int iResult;
//	int recvbuflen = DEFAULT_BUFLEN;
//
//	// Initialize Winsock
//	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (iResult != 0) {
//		printf("WSAStartup failed with error: %d\n", iResult);
//		system("pause");
//		return 1;
//	}
//
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_UNSPEC;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//
//	// Resolve the server address and port
//	iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
//	if (iResult != 0) {
//		printf("getaddrinfo failed with error: %d\n", iResult);
//		WSACleanup();
//		system("pause");
//		return 1;
//	}
//
//	// Attempt to connect to an address until one succeeds
//	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
//
//		// Create a SOCKET for connecting to server
//		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
//			ptr->ai_protocol);
//		if (ConnectSocket == INVALID_SOCKET) {
//			printf("socket failed with error: %ld\n", WSAGetLastError());
//			WSACleanup();
//			system("pause");
//			return 1;
//		}
//
//		// Connect to server.
//		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
//		if (iResult == SOCKET_ERROR) {
//			closesocket(ConnectSocket);
//			ConnectSocket = INVALID_SOCKET;
//			continue;
//		}
//		break;
//	}
//
//	freeaddrinfo(result);
//
//	if (ConnectSocket == INVALID_SOCKET) {
//		printf("Unable to connect to server!\n");
//		WSACleanup();
//		system("pause");
//		return 1;
//	}
//
//	printf("Connected to server! \n");
//
//	// Receive until the peer closes the connection
//	do {
//		// get the string input
//		char str[DEFAULT_BUFLEN];
//		printf("Client Response: ");
//		std::cin >> str;
//
//		// check if the client close connect or not
//		char str2[DEFAULT_BUFLEN];
//		strncpy_s(str2, str, 5);
//
//		if (strcmp(str2, "!exit") == 0)
//		{
//			// shutdown the connection since no more data will be sent
//			iResult = shutdown(ConnectSocket, SD_SEND);
//			if (iResult == SOCKET_ERROR) {
//				printf("shutdown failed with error: %d\n", WSAGetLastError());
//				closesocket(ConnectSocket);
//				WSACleanup();
//				system("pause");
//				return 1;
//			}
//		}
//		else
//		{
//			// Send an initial buffer
//			iResult = send(ConnectSocket, str, (int)strlen(str), 0);
//			if (iResult == SOCKET_ERROR) {
//				printf("send failed with error: %d\n", WSAGetLastError());
//				closesocket(ConnectSocket);
//				WSACleanup();
//				system("pause");
//				return 1;
//			}
//
//			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
//
//			if (iResult > 0)
//			{
//				/*char str2[DEFAULT_BUFLEN];
//				strncpy_s(str2, recvbuf, iResult);*/
//
//				printf("Message length: %d\n", iResult);
//			}
//			else if (iResult == 0)
//				printf("Connection closed\n");
//			else
//				printf("recv failed with error: %d\n", WSAGetLastError());
//		}
//
//	} while (iResult > 0);
//
//
//	// cleanup
//	closesocket(ConnectSocket);
//	WSACleanup();
//
//
//	return true;
//}