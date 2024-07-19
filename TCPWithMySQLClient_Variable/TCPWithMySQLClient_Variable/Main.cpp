#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "../../Socket_err.h"

#pragma comment(lib, "ws2_32")

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 17325
#define BUFSIZE 50


int main()
{
	int Retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, SERVERIP, &ServerSockAddr.sin_addr);

	Retval = connect(ServerSocket, (struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Retval == SOCKET_ERROR) err_quit("connect()");

	char Buf[BUFSIZE];
	const char* TestData[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���õ��� �� �̾߱Ⱑ ���� �� ���׿�",
		"���� �׷��׿�",
	};

	int Len;

	for (int i = 0; i < 4; i++)
	{
		Len = (int)strlen(TestData[i]);
		strncpy(Buf, TestData[i], Len);
		Buf[Len++] = '\n';

		Retval = send(ServerSocket, Buf, Len, 0);
		if (Retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�. \n", Retval);
	}

	//while (1)
	//{
	//	printf("\n[���� ������] ");
	//	if (fgets(Buf, BUFSIZE + 1, stdin) == NULL) break;

	//	Len = (int)strlen(Buf);

	//	if (Buf[Len - 1] == '\n') Buf[Len - 1] = '\0';
	//	if (strlen(Buf) == 0) break;

	//	Retval = send(ServerSocket, Buf, (int)strlen(Buf), 0);
	//	if (Retval == SOCKET_ERROR)
	//	{
	//		err_display("send()");
	//		break;
	//	}

	//	printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�. \n", Retval);

	//	Retval = recv(ServerSocket, Buf, Retval, MSG_WAITALL);
	//	if (Retval == SOCKET_ERROR)
	//	{
	//		err_display("recv()");
	//		break;
	//	}
	//	else if (Retval == 0) break;

	//	Buf[Retval] = '\0';
	//	printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", Retval);
	//	printf("[���� ������] %s\n", Buf);
	//}

	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}