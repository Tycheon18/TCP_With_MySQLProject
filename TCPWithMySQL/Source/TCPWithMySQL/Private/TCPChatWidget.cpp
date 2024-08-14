// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPChatWidget.h"
#include "TCPChatLogActor.h"
#include "Kismet/GameplayStatics.h"

void UTCPChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AActor* ChatActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATCPChatLogActor::StaticClass());

	ATCPChatLogActor* TCPChatActor = Cast<ATCPChatLogActor>(ChatActor);

	if (TCPChatActor)
	{
		ChatLogActor = TCPChatActor;
	}
}
