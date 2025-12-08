// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGS.h"
#include "Net/UnrealNetwork.h"

void AInGameGS::OnRep_AliveCount()
{
	OnChangeAliveCount.ExecuteIfBound(AliveCount);
}

void AInGameGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGameGS, AliveCount);
}

void AInGameGS::UpdateAliveCount(int32 InAliveCount)
{
	AliveCount = InAliveCount;
	OnRep_AliveCount();
}
