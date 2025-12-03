// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"

void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::Start);
	}

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidget::ProcessOnCommit);
		ChatInput->OnTextChanged.AddDynamic(this, &ULobbyWidget::ProcessOnChange);
	}
}

void ULobbyWidget::Start()
{

}

void ULobbyWidget::ProcessOnCommit(const FText& Text, ETextCommit::Type CommitMethod)
{
}

void ULobbyWidget::ProcessOnChange(const FText& Text)
{
}
