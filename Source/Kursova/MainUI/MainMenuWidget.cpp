// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(!BConfirm->OnClicked.IsBound())
	{
		BConfirm->OnClicked.AddDynamic(this, &UMainMenuWidget::ConfirmRegistration);
	}
}

void UMainMenuWidget::ConfirmRegistration()
{
	bool CanContinue(true);
	FString Name;
	FString City;
	FString Difficulty;
	
	Difficulty = CBDifficulty->GetSelectedOption();
	
	if(EName->GetText().IsEmpty())
	{
		TNameError->SetVisibility(ESlateVisibility::Visible);
		CanContinue = false;
	}else
	{
		TNameError->SetVisibility(ESlateVisibility::Hidden);
		Name = EName->GetText().ToString();
	}
	
	if(ECity->GetText().IsEmpty())
	{
		TCityError->SetVisibility(ESlateVisibility::Visible);
		CanContinue = false;
	}else
	{
		TCityError->SetVisibility(ESlateVisibility::Hidden);
		City = ECity->GetText().ToString();
	}

	if(CanContinue)
	{
		SetUserInfo.ExecuteIfBound(Name, City);
		auto Success = SetDifficultyMode.ExecuteIfBound(Difficulty);
	}
	
}
