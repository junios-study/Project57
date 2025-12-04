// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "LobbyGS.h"

#include "../Project57.h"
#include "../Network/NetworkUtil.h"

ALobbyGM::ALobbyGM()
{
}

void ALobbyGM::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("%s Option %s"), *Address, *Options);
}

APlayerController* ALobbyGM::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	//ErrorMessage = TEXT("동준이 나가");

	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	return PC;
}

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


	CheckConnectionCount();

}

void ALobbyGM::BeginPlay()
{
	NET_LOG("Begin");
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(LeftTimerHandle,
		FTimerDelegate::CreateLambda([this]() {
			ALobbyGS* GS = GetGameState<ALobbyGS>();
			if (GS)
			{
				GS->CountDownLeftTime();
			}
		}),
		1.0f,
		true,
		0.0f
	);

	//BeginPlay()보다 UI가 늦어서 업데이트 한번 함.
	CheckConnectionCount();

	NET_LOG("End");
}

void ALobbyGM::CheckConnectionCount()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		//Client다 되는데 서버에서는 업데이트가 안됨
		int32 TempCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator();
			Iter; ++Iter)
		{
			TempCount++;
		}

		GS->ConnectionCount = TempCount;
		GS->OnRep_ConnectionCount();
	}
}

void ALobbyGM::StopTimer()
{
	GetWorldTimerManager().ClearTimer(LeftTimerHandle);
}

void ALobbyGM::StartGame()
{
	StopTimer();
	GetWorld()->ServerTravel(TEXT("InGame"));
}
