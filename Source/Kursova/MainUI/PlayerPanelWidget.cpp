// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPanelWidget.h"
#include "../Public/MainPlayer.h"

void UPlayerPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerPanelWidget::SetWidget(AMainPlayer const* MainPlayer)
{
	TName->SetText(FText::FromString(MainPlayer->GetPlayerName()));
	TCity->SetText(FText::FromString(MainPlayer->GetCity()));
	THitPoints->SetText(FText::FromString(FString::FromInt(MainPlayer->GetHealth())));

	if(MainPlayer->GetGodModeState())
	{
		CIsInGodMode->SetCheckedState(ECheckBoxState::Checked);
	}
}
