// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPCBase.h"
#include "NetworkUtil.h"

void ANetworkPCBase::BeginPlay()
{
	NET_LOG(TEXT("Begin"));

	Super::BeginPlay();

	NET_LOG(TEXT("End"));

}
