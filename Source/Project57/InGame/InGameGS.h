// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InGameGS.generated.h"

DECLARE_DELEGATE_OneParam(FOnChangedAliveCount, const int32);

/**
 * 
 */
UCLASS()
class PROJECT57_API AInGameGS : public AGameStateBase
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void OnRep_AliveCount();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, ReplicatedUsing = "OnRep_AliveCount")
	int32 AliveCount;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateAliveCount(int32 InAliveCount);

	FOnChangedAliveCount OnChangeAliveCount;
	
};
