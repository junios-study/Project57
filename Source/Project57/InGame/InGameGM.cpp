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

	CheckAliveCount();
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
	int32 AliveCount = 0;
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ABaseCharacter* Pawn = Cast<ABaseCharacter>((*Iter)->GetPawn());
		if (Pawn)
		{
			if (Pawn->CurrentHP > 0)
			{
				AliveCount++;
			}
		}
	}

	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		GS->UpdateAliveCount(AliveCount);
	}

	return AliveCount;
}
