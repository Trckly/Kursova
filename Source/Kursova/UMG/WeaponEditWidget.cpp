// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponEditWidget.h"


void UWeaponEditWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ProceedButton->OnClicked.AddDynamic(this, &UWeaponEditWidget::UWeaponEditWidget::Proceed);
	
	CancelButton->OnClicked.AddDynamic(this, &UWeaponEditWidget::UWeaponEditWidget::Cancel);
}

void UWeaponEditWidget::SetupInputBoxes(const TArray<FText>& PropertiesArray)
{
	if(PropertiesArray.Num() < 9)
	{
		return;
	}
	
	ModelEditBox->SetText(PropertiesArray[0]);
	MainTypeEditBox->SetText(PropertiesArray[1]);
	SubtypeEditBox->SetText(PropertiesArray[2]);
	CapacityEditBox->SetText(PropertiesArray[3]);
	ManufacturerEditBox->SetText(PropertiesArray[4]);
	CaliberEditBox->SetText(PropertiesArray[5]);
	LengthEditBox->SetText(PropertiesArray[6]);
	WeightEditBox->SetText(PropertiesArray[7]);
	PriceEditBox->SetText(PropertiesArray[8]);

	ModelEditBox->SetIsEnabled(false);
	MainTypeEditBox->SetIsEnabled(false);
	SubtypeEditBox->SetIsEnabled(false);
	ManufacturerEditBox->SetIsEnabled(false);
	CaliberEditBox->SetIsEnabled(false);

	PreviousModelName = PropertiesArray[0].ToString();
}

void UWeaponEditWidget::Proceed() noexcept
{
	TArray<FText> Result;
	Result.Push(ModelEditBox->GetText());
	Result.Push(MainTypeEditBox->GetText());
	Result.Push(SubtypeEditBox->GetText());
	Result.Push(CapacityEditBox->GetText());
	Result.Push(ManufacturerEditBox->GetText());
	Result.Push(CaliberEditBox->GetText());
	Result.Push(LengthEditBox->GetText());
	Result.Push(WeightEditBox->GetText());
	Result.Push(PriceEditBox->GetText());
	
	OnAcceptedEvent.Execute(Result, PreviousModelName);
	RemoveFromParent();
}

void UWeaponEditWidget::Cancel() noexcept
{
	RemoveFromParent();
}
