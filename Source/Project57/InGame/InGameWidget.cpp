// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "../InGame/InGameGS.h"

void UInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OnChangeAliveCount.BindUObject(this, &UInGameWidget::ProcessChangeAliveCount);
	}
}

void UInGameWidget::ProcessChangeAliveCount(int32 InAliveCount)
{
	if (AliveCount)
	{
		FString Temp = FString::Printf(TEXT("%d 명 생존"), InAliveCount);
		AliveCount->SetText(FText::FromString(Temp));
	}
}
