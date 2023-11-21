// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEditorWidget.h"

void UPlayerEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(!BClose->OnClicked.IsBound())
	{
		BClose->OnClicked.AddDynamic(this, &UPlayerEditorWidget::Exit);
	}

	if(!BSave->OnClicked.IsBound())
	{
		BSave->OnClicked.AddDynamic(this, &UPlayerEditorWidget::SaveStats);
	}
}

void UPlayerEditorWidget::SetBehaviorStats(bool CanMove, bool CanJump, bool CanFire)
{
	if(CanMove)
	{
		CCanMove->SetCheckedState(ECheckBoxState::Checked);
	}
	if(CanJump)
	{
		CCanJump->SetCheckedState(ECheckBoxState::Checked);
	}
	if(CanFire)
	{
		CCanFire->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UPlayerEditorWidget::Exit()
{
	OnCloseButtonClicked.ExecuteIfBound();
}

void UPlayerEditorWidget::SaveStats()
{
	OnSaveButtonClicked.ExecuteIfBound(CCanMove->IsChecked(), CCanJump->IsChecked(), CCanFire->IsChecked());
}
