// Fill out your copyright notice in the Description page of Project Settings.


#include "AdminManagerWidget.h"
#include "../Public/MainPlayer.h"
#include "PlayerPanelWidget.h"
#include "Kismet/GameplayStatics.h"

void UAdminManagerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(!BSearch->OnClicked.IsBound())
	{
		BSearch->OnClicked.AddDynamic(this, &UAdminManagerWidget::FindByName);
	}

	if(!BSort->OnClicked.IsBound())
	{
		BSort->OnClicked.AddDynamic(this, &UAdminManagerWidget::SortByCity);
	}

	if(!BEditBehavior->OnClicked.IsBound())
	{
		BEditBehavior->OnClicked.AddDynamic(this, &UAdminManagerWidget::EditPlayerBehavior);
	}

	PlayerIndex = 0;
}

void UAdminManagerWidget::SetPlayers()
{
	TArray<AActor*> PlayersArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainPlayer::StaticClass(), PlayersArr);
	
	APlayerController* Controller = Cast<APlayerController>(GetOwningPlayer());
	if(!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to get PlayerController in AdminManagerWidget"));
		return;
	}

	for(AActor* Player : PlayersArr)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(Player);
		UPlayerPanelWidget* PanelWidget = CreateWidget<UPlayerPanelWidget>(Controller, PlayerPanelWidgetClass);

		if(PanelWidget)
		{
			PanelWidget->SetWidget(MainPlayer);
			MainPlayer->SetPlayerIndex(PlayerIndex++);
			MapOfPlayers.Add(MainPlayer->GetPlayerIndex(), PanelWidget);
			SListOfPlayers->AddChild(PanelWidget);
		}
	}
}

void UAdminManagerWidget::FindByName()
{
	
}

void UAdminManagerWidget::SortByCity()
{
}

void UAdminManagerWidget::EditPlayerBehavior()
{
}
