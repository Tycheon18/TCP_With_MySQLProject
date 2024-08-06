// Fill out your copyright notice in the Description page of Project Settings.


#include "FTCPThread.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "IPAddress.h"

FTCPThread::FTCPThread()
{
	ServerAddress = nullptr;
	Thread = nullptr;
	bisRunThread = false;
	bisThreadRunning = false;
}

FTCPThread::~FTCPThread()
{
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

bool FTCPThread::Init()
{
	bisRunThread = true;

	ServerSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("DefaultSocket"), false);
	ServerAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	int32 Port = 17325;

	FString IP = TEXT("127.0.0.1");
	FIPv4Address TemporaryAddr;
	FIPv4Address::Parse(IP, TemporaryAddr);

	ServerAddress->SetPort(Port);
	ServerAddress->SetIp(TemporaryAddr.Value);

	if (ServerSocket->Connect(*ServerAddress))
	{

	}
	else
	{

	}

	return true;
}

uint32 FTCPThread::Run()
{
	bisThreadRunning = true;

	while (bisRunThread)
	{
		if (ServerSocket->GetConnectionState() != ESocketConnectionState::SCS_Connected) break;

		RecvText = "";

		uint8_t RecvBuf[512];
		int32 RecvByte = 0;

		ServerSocket->Recv(RecvBuf, 512, RecvByte);

		char* RecvClientText = new char[512];
		memset(RecvClientText, '\0', strlen(RecvClientText));

		for (int i = 0; i < std::size(RecvBuf); ++i)
		{
			RecvClientText[i] = RecvBuf[i];
		}

		RecvText = RecvClientText;

		FPlatformProcess::Sleep(0.1f);
	}

	return 0;
}

void FTCPThread::Stop()
{
	bisRunThread = false;
}

bool FTCPThread::IsThreadRunning() const
{
	return bisThreadRunning;
}
