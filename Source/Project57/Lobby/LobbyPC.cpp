// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGS.h"
#include "LobbyGM.h"

ALobbyPC::ALobbyPC()
{
}

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (LobbyWidgetClass)
	{
		if (IsLocalPlayerController())
		{
			LobbyWidgetObject = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
			LobbyWidgetObject->AddToViewport();

			//Listen Server일때만 실행
			ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GM)
			{
				GM->CheckConnectionCount();
			}
		}
	}
}

void ALobbyPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
