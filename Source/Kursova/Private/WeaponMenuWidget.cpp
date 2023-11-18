// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMenuWidget.h"
#include "Kursova/Miscellaneous/Algorithms.h"
#include "MainPlayer.h"

void UWeaponMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SortButton->OnClicked.AddDynamic(this, &UWeaponMenuWidget::SortByCaliber);
	EditButton->OnClicked.AddDynamic(this, &UWeaponMenuWidget::EditProperties);

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
			if(WidgetCreated)
			{
				WidgetCreated->InitWithData(WeaponInstance->GetStructure());
				ChildRowWidgets.Push(WidgetCreated);
				WeaponContent->AddChild(WidgetCreated);
				if(!WidgetCreated->IsInViewport() && !WidgetCreated->IsVisible())
				{
					WidgetCreated->AddToViewport();
				}
			}
		}
	}
}

void UWeaponMenuWidget::SortByCaliber()
{
	TArray<UWidget*> ChildrenTemp = WeaponContent->GetAllChildren();
	WeaponContent->ClearChildren();
	TArray<UWeaponDataWidget*> Children;
	for(auto Child : ChildrenTemp)
	{
		Children.Push(Cast<UWeaponDataWidget>(Child));
	}
	
	ShellSort(Children);

	for(auto Child : Children)
	{
		WeaponContent->AddChild(Child);
	}
	
}

void UWeaponMenuWidget::EditProperties()
{
	UWeaponEditWidget* CreatedWidget = CreateWidget<UWeaponEditWidget>(this, WeaponEditWidgetClass);
	CreatedWidget->AddToViewport();
}

void UWeaponMenuWidget::ShellSort(TArray<UWeaponDataWidget*>& Children)
{
	for (int Gap = Children.Num() / 2; Gap > 0; Gap /= 2)
	{
		for (int i = Gap; i < Children.Num(); ++i)
		{
			auto Temp = Children[i];

			int j;
			for(j = i; j >= Gap && Children[j-Gap]->GetCaliber() > Temp->GetCaliber(); j -= Gap)
				Children[j] = Children[j-Gap];
			
			Children[j] = Temp;
		}
	}
}
