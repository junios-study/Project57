// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGS.h"

#include "../Project57.h"
#include "../Network/NetworkUtil.h"

void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	NET_LOG("Begin");


	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::Start);
	}

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidget::ProcessOnCommit);
		ChatInput->OnTextChanged.AddDynamic(this, &ULobbyWidget::ProcessOnChange);
	}

	
	ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OnChageLeftTime.AddDynamic(this, &ULobbyWidget::UpdateLeftTime);
		GS->OnChangetConnectionCount.AddDynamic(this, &ULobbyWidget::UpdateConnectionCount);
	}

	NET_LOG("End");
}

void ULobbyWidget::UpdateConnectionCount(int32 InConnectionCount)
{
	if (ConnectionCount)
	{
		FString Message = FString::Printf(TEXT("%d명 접속"), InConnectionCount);
		ConnectionCount->SetText(FText::FromString(Message));
	}
}

void ULobbyWidget::Start()
{
	GetWorld()->ServerTravel(TEXT("InGame"));
}

void ULobbyWidget::ProcessOnCommit(const FText& Text, ETextCommit::Type CommitMethod)
{
}

void ULobbyWidget::ProcessOnChange(const FText& Text)
{
}

void ULobbyWidget::UpdateLeftTime(int32 InLeftTime)
{
	if (LeftTime)
	{
		FString Message = FString::Printf(TEXT("%d초 남음"), InLeftTime);
		LeftTime->SetText(FText::FromString(Message));
	}
}


