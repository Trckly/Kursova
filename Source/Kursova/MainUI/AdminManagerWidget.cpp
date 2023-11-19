// Fill out your copyright notice in the Description page of Project Settings.


#include "AdminManagerWidget.h"
#include "../Public/MainPlayer.h"
#include "PlayerPanelWidget.h"

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

void UAdminManagerWidget::SetPlayers(TArray<AMainPlayer*> ArrOfPlayers)
{
	APlayerController* Controller = Cast<APlayerController>(GetOwningPlayer());
	if(!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to get PlayerController in AdminManagerWidget"));
		return;
	}

	for(AMainPlayer* Player : ArrOfPlayers)
	{
		UPlayerPanelWidget* PanelWidget = CreateWidget<UPlayerPanelWidget>(Controller, PlayerPanelWidgetClass);

		if(PanelWidget)
		{
			PanelWidget->SetWidget(Player);
			MapOfPlayers.Add(Player->GetPlayerIndex(), PanelWidget);
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
