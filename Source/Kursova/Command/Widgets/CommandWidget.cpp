// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandWidget.h"


void UCommandWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PosRotationButton->OnClicked.AddDynamic(this, &UCommandWidget::PositiveRotation);
	NegRotationButton->OnClicked.AddDynamic(this, &UCommandWidget::NegativeRotation);
}

void UCommandWidget::PositiveRotation()
{
	if(PositiveRotationDelegate.IsBound())
	{
		PositiveRotationDelegate.Execute(Actor);
		CloseWidget();
	}
}

void UCommandWidget::NegativeRotation()
{
	if(NegativeRotationDelegate.IsBound())
	{
		NegativeRotationDelegate.Execute(Actor);
		CloseWidget();
	}
}

void UCommandWidget::CloseWidget()
{
	RemoveFromViewport();
	if(WidgetClosed.IsBound())
	{
		WidgetClosed.Execute();
	}
}

void UCommandWidget::PassActor(AActor* HitActor)
{
	Actor = HitActor;
}
