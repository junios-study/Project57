// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/UnrealNetwork.h"
#include "LobbyPC.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyWidget.h"
#include "../Project57.h"
#include "../Network/NetworkUtil.h"
#include "LobbyGM.h"


void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);
	DOREPLIFETIME(ALobbyGS, ConnectionCount);
}

void ALobbyGS::OnRep_LeftTime()
{
	//ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//if (PC && PC->LobbyWidgetObject)
	//{
	//	PC->LobbyWidgetObject->UpdateLeftTime(LeftTime);
	//}
	OnChageLeftTime.Broadcast(LeftTime);
	
}

void ALobbyGS::OnRep_ConnectionCount()
{
	OnChangetConnectionCount.Broadcast(ConnectionCount);
}

void ALobbyGS::BeginPlay()
{
	NET_LOG("Begin"); // PC->BeginPlay()
	Super::BeginPlay();
	NET_LOG("End");
}

void ALobbyGS::CountDownLeftTime()
{
	//Server
	if (LeftTime > 0)
	{
		LeftTime--;
		OnRep_LeftTime();
	}
	else
	{
		ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->StartGame();
		}
	}
}

