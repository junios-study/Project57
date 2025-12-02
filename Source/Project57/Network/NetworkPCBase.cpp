// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPCBase.h"
#include "NetworkUtil.h"

void ANetworkPCBase::BeginPlay()
{
	NET_LOG(TEXT("Begin"));

	Super::BeginPlay();

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		NET_LOG(TEXT("LocalRole::ROLE_Authority"));
	}
	else if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		NET_LOG(TEXT("LocalRole::ROLE_AutonomousProxy"));
	}
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	{
		NET_LOG(TEXT("LocalRole::ROLE_SimulatedProxy"));
	}

	if (GetRemoteRole() == ENetRole::ROLE_Authority)
	{
		NET_LOG(TEXT("RmoteRole::ROLE_Authority"));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
	{
		NET_LOG(TEXT("RmoteRole::ROLE_AutonomousProxy"));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
	{
		NET_LOG(TEXT("RmoteRole::ROLE_SimulatedProxy"));
	}

	NET_LOG(TEXT("End"));

}

void ANetworkPCBase::PostNetInit()
{
	NET_LOG(TEXT("Begin"));

	Super::PostNetInit();

	NET_LOG(TEXT("End"));
}

void ANetworkPCBase::PostInitializeComponents()
{
	NET_LOG(TEXT("Begin"));

	Super::PostInitializeComponents();

	NET_LOG(TEXT("End"));
}
