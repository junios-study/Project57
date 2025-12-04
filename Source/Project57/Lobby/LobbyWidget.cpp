// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGS.h"
#include "LobbyPC.h"
#include "../Title/DataGameInstanceSubsystem.h"
#include "LobbyGM.h"

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

void ULobbyWidget::AddMessage(const FText& Message)
{
	if (ChatScrollBox)
	{
		URichTextBlock* NewMessageBlock = NewObject<URichTextBlock>(ChatScrollBox);
		if (NewMessageBlock)
		{
			
			NewMessageBlock->SetText(Message);
			NewMessageBlock->SetAutoWrapText(true);
			NewMessageBlock->SetWrapTextAt(ChatScrollBox->GetCachedGeometry().GetLocalSize().X);
			NewMessageBlock->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);

			//FSlateFontInfo FontInfo =  NewMessageBlock->GetFont();
			//FontInfo.Size = 20;
			//NewMessageBlock->SetFont(FontInfo);
			//NewMessageBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0, 0, 1)));
			if (ChatStyleSet)
			{
				NewMessageBlock->SetTextStyleSet(ChatStyleSet);
			}
			
			ChatScrollBox->AddChild(NewMessageBlock);
			ChatScrollBox->ScrollToEnd();
		}
	}

}

void ULobbyWidget::ShowStartButton()
{
	if (StartButton)
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget::Start()
{
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->StartGame();
	}
}

void ULobbyWidget::ProcessOnCommit(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
		case ETextCommit::OnEnter:
		{
			ALobbyPC* PC = Cast<ALobbyPC>(GetOwningPlayer());
			if (PC)
			{
				UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
				if (GI)
				{
					UDataGameInstanceSubsystem* MySubsystem = GI->GetSubsystem<UDataGameInstanceSubsystem>();
					FString Temp = FString::Printf(TEXT("%s : %s"),
						*MySubsystem->UserID, *Text.ToString());

					//Local PC call -> Server PC execute
					PC->C2S_SendMessage(FText::FromString(Temp));
					ChatInput->SetText(FText::FromString(TEXT("")));
				}
			}	

		}
		break;

		case ETextCommit::OnCleared:
		{
			ChatInput->SetUserFocus(GetOwningPlayer());
		}
		break;
	}
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


