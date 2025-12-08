// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameGM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API AInGameGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AInGameGM();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	int32 CheckAliveCount();

};
