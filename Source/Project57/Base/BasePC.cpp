// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePC.h"
#include "../InGame/InGameWidget.h"

ABasePC::ABasePC()
{
}

void ABasePC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (UITemplate)
		{
			UIObject = CreateWidget<UInGameWidget>(this, UITemplate);
			UIObject->AddToViewport();
		}
	}
}
