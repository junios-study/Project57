// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGM.h"
#include "../Base/BaseCharacter.h"
#include "InGameGS.H"


AInGameGM::AInGameGM()
{
}

void AInGameGM::BeginPlay()
{
	Super::BeginPlay();
}



void AInGameGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CheckAliveCount();
}

void AInGameGM::Logout(AController* Exiting)
{
	CheckAliveCount();

	Super::Logout(Exiting);
}

int32 AInGameGM::CheckAliveCount()
{
	int32 PlayerControllerCount = 0;
	int32 AliveCount = 0;
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		++PlayerControllerCount;
		ABaseCharacter* Pawn = Cast<ABaseCharacter>((*Iter)->GetPawn());
		if (Pawn)
		{
			if (Pawn->CurrentHP > 0)
			{
				AliveCount++;
			}
		}
	}
	//PlayerController 갯수가 2개 이상이면 게임중, 생존 1일때는 종료

	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		GS->UpdateAliveCount(AliveCount);
	}

	FTimerHandle EndTimer;

	if (PlayerControllerCount >= 2 && AliveCount == 1)
	{
		GetWorld()->GetTimerManager().SetTimer(EndTimer,
			FTimerDelegate::CreateLambda([&]() {
				GetWorld()->GetTimerManager().ClearTimer(EndTimer);
				GetWorld()->ServerTravel(TEXT("Lobby"));
				}),
			10.0f,
			false,
			-1.0f
		);
	}

	return AliveCount;
}
