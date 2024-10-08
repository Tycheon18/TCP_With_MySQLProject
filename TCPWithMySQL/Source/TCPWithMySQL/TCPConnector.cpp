// Fill out your copyright notice in the Description page of Project Settings.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "TCPConnector.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#include <WinSock2.h>
#include <iostream>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"


#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"

#include "Networking.h"
#include "SocketSubsystemModule.h"

#include "FTCPThread.h"

#pragma comment(lib, "ws2_32.lib")

// Sets default values
ATCPConnector::ATCPConnector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ClientSocket = nullptr;
	UnrealThread = nullptr;

	JsonData.Name = "";
	JsonData.Description = "";
	JsonData.Color = "";
	JsonData.Number = -1;
}

// Called when the game starts or when spawned
void ATCPConnector::BeginPlay()
{
	Super::BeginPlay();

	if (ConnectServer())
	{
		SendText();
	}
	else
	{

	}
	
}

// Called every frame
void ATCPConnector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATCPConnector::ConnectServer()
{
	ClientSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("DefaultSocket"), false);
	ClientAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	int32 Port = 17325;

	FString IP = TEXT("127.0.0.1");
	FIPv4Address TemporaryAddr;
	FIPv4Address::Parse(IP, TemporaryAddr);

	ClientAddress->SetPort(Port);
	ClientAddress->SetIp(TemporaryAddr.Value);

	return (ClientSocket->Connect(*ClientAddress));;
}

void ATCPConnector::SendText()
{
	//TChar* ClientText = new char[512];
	//Text = "Umjunsik is Alive";
	//ClientText = TCHAR_TO_UTF8(*Text);
	if(Text != "")
	{
		FString Serial = "C|" + Text + "|#";

		TCHAR* SerializedText = Serial.GetCharArray().GetData();

		//uint8 Buffer[512];
		//memset(Buffer, '\0', std::size(Buffer));

		//for (int i = 0; i < strlen(ClientText); ++i)
		//{
		//	Buffer[i] = ClientText[i];
		//}

		int32 Size = FCString::Strlen(SerializedText);

		int32 BytesSent = 0;

		ClientSocket->Send((uint8*)TCHAR_TO_UTF8(SerializedText), Size, BytesSent);

		TArray<uint8> ReceivedData;
		uint32 RecvSize = 0;

		if(ClientSocket->HasPendingData(RecvSize))
		{
			ReceivedData.Init(0, RecvSize);
			int32 Read = 0;
			ClientSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

			FString RecvMessage = FString(UTF8_TO_TCHAR(ReceivedData.GetData()));
		}
	}
}

void ATCPConnector::SendDataText()
{
	CreateDataText();

	if (DataText != "")
	{
		FString Serial = "Q" + DataText;

		TCHAR* SerializedText = Serial.GetCharArray().GetData();

		int32 Size = FCString::Strlen(SerializedText);

		int32 BytesSent = 0;

		ClientSocket->Send((uint8*)TCHAR_TO_UTF8(SerializedText), Size, BytesSent);
	}
}

void ATCPConnector::SendPacket()
{
	TSharedPtr<FBufferArchive> Packet = CreatePacket(0, TEXT("start packet"));

	//AsyncTask(ENamedThreads::AnyThread, [this, Packet]()
	//	{
	//		if (ClientSocket == nullptr || this == nullptr)
	//		{
	//			return;
	//		}

			int32 NumSend;
			bool bSuccess = ClientSocket->Send(Packet->GetData(), Packet->Num(), NumSend);
//		});
}

void ATCPConnector::CreateDataText()
{
	if ((JsonData.Name == "") || (JsonData.Description == "") || (JsonData.Color == "") ||
		(JsonData.Number == -1))
	{
		return;
	}

	DataText = JsonData.Name + ',' + JsonData.Description + ',' + JsonData.Color + ',' + FString::FromInt(JsonData.Number);
}

TSharedPtr<FBufferArchive> ATCPConnector::CreatePacket(int32 InType, const uint8* InPayload, int32 InPayloadSize)
{
	FMessageHeader Header(InType, InPayloadSize);
	constexpr static int32 HeaderSize = sizeof(FMessageHeader);

	TSharedPtr<FBufferArchive> Packet = MakeShareable(new FBufferArchive());

	(*Packet) << Header;

	Packet->Append(InPayload, InPayloadSize);

	return Packet;
}

TSharedPtr<FBufferArchive> ATCPConnector::CreatePacket(int32 Type, const FString& SendText)
{
	SCOPE_CYCLE_COUNTER(STAT_Send);
	
	FTCHARToUTF8 Convert(*SendText);
	FArrayWriter WriterArray;

	WriterArray.Serialize((UTF8CHAR*)Convert.Get(), Convert.Length());

	TSharedPtr<FBufferArchive> Packet = CreatePacket(Type, WriterArray.GetData(), WriterArray.Num());

	return Packet;
}

void ATCPConnector::StartThread()
{
}

void ATCPConnector::SendToThread()
{
}

void ATCPConnector::ReceiveToThread()
{
}

void ATCPConnector::StopThread()
{
}

void ATCPConnector::Send()
{
}

bool ATCPConnector::Receive(FSocket* Socket, uint8* Results, int32 Size)
{
	int32 Offset = 0;
	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, NumRead);
		check(NumRead <= Size);

		if (NumRead <= 0)
		{
			return false;
		}

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}

