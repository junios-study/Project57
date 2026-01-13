// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"



#include "DataGameInstanceSubsystem.generated.h"


//struct FHttpRequestPtr;
//struct FHttpResponsePtr;


USTRUCT(BlueprintType)
struct FLoginData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool result;
};


/**
 *
 */
UCLASS()
class PROJECT57_API UDataGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString UserID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString Password;

	UFUNCTION(BlueprintCallable)
	void Login();

	FHttpModule* HTTPModule;

	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);


	void OnProcessLLMRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);

};
