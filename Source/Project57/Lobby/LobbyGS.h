// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	int32 LeftTime = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	int32 ConnectionCount = 0;

};
