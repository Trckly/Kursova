// Fill out your copyright notice in the Description page of Project Settings.


#include "Kursova/UMG/WeaponDataWidget.h"

void UWeaponDataWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(!SelectionButton->OnClicked.IsBound())
	{
		SelectionButton->OnClicked.AddDynamic(this, &UWeaponDataWidget::RowSelected);
	}
	
	FString Manufacturer =  ManufacturerText->GetText().ToString();
	if(Manufacturer.Len() > GString_Clipping_Len)
	{
		StartAnimation();
	}
}

void UWeaponDataWidget::InitWithData(const FWeaponUnit& WeaponUnit)
{
	if(!WeaponUnit.Model.IsEmpty())
	{
		ModelText->SetText(FText::FromString(WeaponUnit.Model));
		MainTypeText->SetText(FText::FromString(WeaponUnit.MainType));
		SubtypeText->SetText(FText::FromString(WeaponUnit.Subtype));
		CapacityText->SetText(FText::FromString(FString::FromInt(WeaponUnit.Capacity)));
		ManufacturerText->SetText(FText::FromString(WeaponUnit.Manufacturer));
		CaliberText->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), WeaponUnit.Caliber)));
		LengthText->SetText(FText::FromString(FString::FromInt(WeaponUnit.Length)));
		WeightText->SetText(FText::FromString(FString::FromInt(WeaponUnit.Weight)));
		PriceText->SetText(FText::FromString(FString::FromInt(WeaponUnit.Price)));
	}
}

float UWeaponDataWidget::GetCaliber()
{
	return FCString::Atof(*CaliberText->GetText().ToString());
}

FString UWeaponDataWidget::GetModelName()
{
	return ModelText->GetText().ToString();
}

FString UWeaponDataWidget::GetMainTypeName()
{
	return MainTypeText->GetText().ToString();
}

FString UWeaponDataWidget::GetSubtypeName()
{
	return SubtypeText->GetText().ToString();
}

FString UWeaponDataWidget::GetManufacturerName()
{
	return ManufacturerText->GetText().ToString();
}

TArray<FText> UWeaponDataWidget::GetAllProperties()
{
	TArray<FText> Result;
	Result.Push(ModelText->GetText());
	Result.Push(MainTypeText->GetText());
	Result.Push(SubtypeText->GetText());
	Result.Push(CapacityText->GetText());
	Result.Push(ManufacturerText->GetText());
	Result.Push(CaliberText->GetText());
	Result.Push(LengthText->GetText());
	Result.Push(WeightText->GetText());
	Result.Push(PriceText->GetText());

	return Result;
}

void UWeaponDataWidget::RowSelected()
{
	if(!OnRowClicked.ExecuteIfBound(ModelText->GetText().ToString()))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRowClickedEvent is not bound!"));
	}
}


