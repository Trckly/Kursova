// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataWidget.h"

void UWeaponDataWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FString Manufacturer =  ManufacturerText->GetText().ToString();
	if(Manufacturer.Len() > StringClippingLength)
	{
		StartAnimation();
	}
}

void UWeaponDataWidget::InitWithData(const FWeaponUnit& WeaponUnit)
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

