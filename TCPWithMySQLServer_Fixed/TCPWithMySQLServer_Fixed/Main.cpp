

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "../../Socket_err.h"

#pragma comment(lib, "ws2_32")

using namespace std;

#define SERVERPORT 17325
#define BUFSIZE 50

int main()
{
	int Retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	printf("[알림] WinSock 초기화 성공\n");

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (ServerSocket == INVALID_SOCKET) err_quit("socket()");
	printf("[알림] Socket 생성 성공\n");

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = INADDR_ANY;
	ServerSockAddr.sin_port = htons(SERVERPORT);

	Retval = bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Retval == SOCKET_ERROR) err_quit("bind()");

	Retval = listen(ServerSocket, SOMAXCONN);
	if (Retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET ClientSocket;
	SOCKADDR_IN ClientSockAddr;
	int AddrLen;
	char Buf[BUFSIZE + 1];

	while (1)
	{
		AddrLen = sizeof(ClientSockAddr);
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientSockAddr, &AddrLen);
		if (ClientSocket == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		char Addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ClientSockAddr.sin_addr, Addr, sizeof(Addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d\n", Addr, ntohs(ClientSockAddr.sin_port));

		while (1)
		{

			Retval = recv(ClientSocket, Buf, BUFSIZE, MSG_WAITALL);
			if (Retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (Retval == 0)
			{
				break;
			}

			Buf[Retval] = '\0';
			printf("[TCP/%s : %d] %s\n", Addr, ntohs(ClientSockAddr.sin_port), Buf);

			Retval = send(ClientSocket, Buf, Retval, 0);
			if (Retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
		}

		closesocket(ClientSocket);
		printf("[TCP 서버] 클라이언트 종료 : IP 주소 = %s , 포트 번호 = %d\n", Addr, ntohs(ClientSockAddr.sin_port));
	}

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}