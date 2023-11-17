// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMenuWidget.h"

#include "MainPlayer.h"

void UWeaponMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AMainPlayer* CurrentPlayer = Cast<AMainPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(!CurrentPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Main player cast failed!"));
	}
	else
	{
		TArray<AWeaponClass*> ActorPickedWeapons = CurrentPlayer->GetAllPickedWeapons();
		for (auto WeaponInstance : ActorPickedWeapons)
		{
			auto WidgetCreated = CreateWidget<UWeaponDataWidget>(this, WeaponDataWidgetClass);
			WidgetCreated->InitWithData(WeaponInstance->GetStructure());
			WeaponContent->AddChild(WidgetCreated);
			if(!WidgetCreated->IsInViewport() && !WidgetCreated->IsVisible())
			{
				WidgetCreated->AddToViewport();
			}
		}
	}
}
