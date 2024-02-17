

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <WinSock2.h>

#include "Socket_err.h"

#pragma comment(lib, "ws2_32")

using namespace std;

#define SERVERPORT 8080
#define BUFSIZE 512

int main()
{
	int Retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	printf("[알림] WinSock 초기화 성공\n");

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (ServerSocket == INVALID_SOCKET) err_quit("socket()");
	printf("[알림] Socket 생성 성공\n");

	closesocket(ServerSocket);

	//SOCKADDR_IN ServerSockAddr;
	//memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	//ServerSockAddr.sin_family = PF_INET;
	//ServerSockAddr.sin_addr.s_addr = INADDR_ANY;
	//ServerSockAddr.sin_port = htons(SERVERPORT);

	//Retval = bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	//if (Retval == SOCKET_ERROR) err_quit("listen()");

	WSACleanup();

	return 0;
}