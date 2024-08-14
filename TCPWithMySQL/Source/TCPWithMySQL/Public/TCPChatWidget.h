// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TCPChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class TCPWITHMYSQL_API UTCPChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


protected:

	UPROPERTY()
	class ATCPChatLogActor* ChatLogActor;
};
