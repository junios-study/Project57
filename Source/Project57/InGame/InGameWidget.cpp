// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "../InGame/InGameGS.h"
#include "../InGame/InGameGM.h"
#include "../Base/BaseCharacter.h"

void UInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OnChangeAliveCount.BindUObject(this, &UInGameWidget::ProcessChangeAliveCount);
	}

	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CheckAliveCount();
	}

	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	if (PC)
	{
		ABaseCharacter* Pawn = Cast<ABaseCharacter>(PC->GetPawn());
		if (Pawn)
		{
			Pawn->OnHPChanged.AddDynamic(this, &UInGameWidget::ProcessHPBar); 
			Pawn->OnRep_CurrntHP();
		}
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

void UInGameWidget::ProcessHPBar(float InPercent)
{
	if (HPBar)
	{
		HPBar->SetPercent(InPercent);
	}
}
