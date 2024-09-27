// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandWidget.h"

#include "Kursova/ObjectDecorator/Cube.h"
#include "Kursova/State/GreenState.h"
#include "Kursova/State/RedState.h"


void UCommandWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PosRotationButton->OnClicked.AddDynamic(this, &UCommandWidget::PositiveRotation);
	NegRotationButton->OnClicked.AddDynamic(this, &UCommandWidget::NegativeRotation);

	RedStateButton->OnClicked.AddDynamic(this, &UCommandWidget::RedState);
	GreenStateButton->OnClicked.AddDynamic(this, &UCommandWidget::GreenState);
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

void UCommandWidget::RedState()
{
	ACube* Cube = Cast<ACube>(Actor);
	if(Cube)
	{
		Cube->ChangeState(NewObject<URedState>());
	}
	
	CloseWidget();
}

void UCommandWidget::GreenState()
{
	ACube* Cube = Cast<ACube>(Actor);
	if(Cube)
	{
		Cube->ChangeState(NewObject<UGreenState>());
	}

	CloseWidget();
}

void UCommandWidget::CloseWidget()
{
	RemoveFromParent();
	if(WidgetClosed.IsBound())
	{
		WidgetClosed.Execute();
	}
}

void UCommandWidget::PassActor(AActor* HitActor)
{
	Actor = HitActor;
}
