// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGMBase.h"
#include "../Project57.h"
#include "../Network/NetworkUtil.h"

void ANetworkGMBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* ANetworkGMBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void ANetworkGMBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ANetworkGMBase::BeginPlay()
{
	NET_LOG(TEXT("Hello"));

	Super::BeginPlay();

}
