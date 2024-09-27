// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPanelWidget.h"
#include "../Public/MainPlayer.h"

void UPlayerPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(!BSetActive->OnClicked.IsBound())
	{
		BSetActive->OnClicked.AddDynamic(this, &UPlayerPanelWidget::SetActive);
	}

	if(!CIsInGodMode->OnCheckStateChanged.IsBound())
	{
		CIsInGodMode->OnCheckStateChanged.AddDynamic(this, &UPlayerPanelWidget::SetGodMode);
	}

	StandardBackgroundColor = BSetActive->GetBackgroundColor();
}

void UPlayerPanelWidget::SetWidget(AMainPlayer* MainPlayer, int Index)
{
	TName->SetText(FText::FromString(MainPlayer->GetPlayerName()));
	TCity->SetText(FText::FromString(MainPlayer->GetCity()));
	THitPoints->SetText(FText::FromString(FString::FromInt(MainPlayer->GetHealth())));
	
	if(MainPlayer->GetGodModeState())
	{
		CIsInGodMode->SetCheckedState(ECheckBoxState::Checked);
	}

	PlayerIndex = Index;
	Player = MainPlayer;
}

FString UPlayerPanelWidget::GetName()
{
	return TName->GetText().ToString();
}

FString UPlayerPanelWidget::GetCity()
{
	return TCity->GetText().ToString();
}

int UPlayerPanelWidget::GetIndex()
{
	return PlayerIndex;
}

void UPlayerPanelWidget::SetActive()
{
	SetWidgetActive.ExecuteIfBound(PlayerIndex);
}

void UPlayerPanelWidget::SetGodMode(bool IsGodModeSet)
{
	Player->Multicast_SetGodMode(IsGodModeSet);
}

void UPlayerPanelWidget::SetBehavior(bool CanMove, bool CanJump, bool CanFire)
{
	Player->Multicast_SetBehavior(CanMove, CanJump, CanFire);
}

bool UPlayerPanelWidget::GetAbilityToMove()
{
	return Player->GetBehaviorSet().CanMove;
}

bool UPlayerPanelWidget::GetAbilityToJump()
{
	return Player->GetBehaviorSet().CanJump;
}

bool UPlayerPanelWidget::GetAbilityToFire()
{
	return Player->GetBehaviorSet().CanShoot;
}

void UPlayerPanelWidget::ResetBackgroundColor()
{
	BSetActive->SetBackgroundColor(StandardBackgroundColor);
}

void UPlayerPanelWidget::SetBackgroundColor()
{
	BSetActive->SetBackgroundColor(FLinearColor::Gray);
}

