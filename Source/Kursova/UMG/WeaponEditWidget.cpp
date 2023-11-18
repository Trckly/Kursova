// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponEditWidget.h"

void UWeaponEditWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ProceedButton->OnClicked.AddDynamic(this, &UWeaponEditWidget::UWeaponEditWidget::Proceed);
	
	CancelButton->OnClicked.AddDynamic(this, &UWeaponEditWidget::UWeaponEditWidget::Cancel);
}

void UWeaponEditWidget::SetupInputBoxes()
{
}

void UWeaponEditWidget::Proceed()
{
	RemoveFromParent();
}

void UWeaponEditWidget::Cancel()
{
	RemoveFromParent();
}
