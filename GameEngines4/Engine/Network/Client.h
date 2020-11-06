#ifndef CLIENT_H
#define CLIENT_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"
class Client
{
public:
	Client();
	~Client();
	void sendMsg();
	bool OnCreate();
private:

};


#endif // !CLIENT_H

