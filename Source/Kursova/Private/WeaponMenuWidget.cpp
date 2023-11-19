// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMenuWidget.h"

void UWeaponMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SortButton->OnClicked.AddDynamic(this, &UWeaponMenuWidget::SortByCaliber);
	EditButton->OnClicked.AddDynamic(this, &UWeaponMenuWidget::CreatePropertiesEditor);
}

void UWeaponMenuWidget::RemoveFromParent()
{
	Super::RemoveFromParent();

	if(EditWidget)
	{
		EditWidget->RemoveFromParent();
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

void UWeaponMenuWidget::CreatePropertiesEditor()
{
	if(SelectedWidget)
	{
		UWeaponEditWidget* CreatedWidget = CreateWidget<UWeaponEditWidget>(this, WeaponEditWidgetClass);
		if(CreatedWidget)
		{
			EditWidget = CreatedWidget;
			EditWidget->OnAcceptedEvent.BindDynamic(this, & UWeaponMenuWidget::EditWeaponUnitProperties);
			EditWidget->SetupInputBoxes(SelectedWidget->GetAllProperties());
			EditWidget->AddToViewport();
			
		}
	}
}

void UWeaponMenuWidget::EditWeaponUnitProperties(const TArray<FText>& ChangedValues, const FString& OriginalModelName)
{
	for (int i = 0; i < ActorPickedWeapons.Num(); ++i)
	{
		if(ActorPickedWeapons[i]->GetStructure().Model == OriginalModelName)
		{
			ActorPickedWeapons[i]->EditStructure(ChangedValues);
		}
	}

	WeaponContent->ClearChildren();
	ChildRowWidgets.Empty();
	for (auto WeaponInstance : ActorPickedWeapons)
	{
		auto WidgetCreated = CreateWidget<UWeaponDataWidget>(this, WeaponDataWidgetClass);
		if(WidgetCreated)
		{
			WidgetCreated->InitWithData(WeaponInstance->GetStructure());
			WeaponContent->AddChild(WidgetCreated);
				
			ChildRowWidgets.Push(WidgetCreated);
			WidgetCreated->OnRowClicked.BindDynamic(this, &UWeaponMenuWidget::HandleSelectedRowWidget);
		}
	}
}

void UWeaponMenuWidget::HandleSelectedRowWidget(const FString& ModelName)
{
	for (auto Child : ChildRowWidgets)
	{
		if(ModelName == Child->GetModelName())
		{
			SelectedWidget = Child;
			SelectedWidget->SetButtonEnabledColor();
		}
		else
		{
			Child->SetButtonDisabledColor();
		}
	}
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

void UWeaponMenuWidget::SetActorPickedWeapons(const TArray<AWeaponClass*>& PickedWeapons)
{
	for (auto Weapon : PickedWeapons)
	{
		ActorPickedWeapons.Push(Weapon);
	}

	for (auto WeaponInstance : ActorPickedWeapons)
	{
		auto WidgetCreated = CreateWidget<UWeaponDataWidget>(this, WeaponDataWidgetClass);
		if(WidgetCreated)
		{
			WidgetCreated->InitWithData(WeaponInstance->GetStructure());
			WeaponContent->AddChild(WidgetCreated);
				
			ChildRowWidgets.Push(WidgetCreated);
			WidgetCreated->OnRowClicked.BindDynamic(this, &UWeaponMenuWidget::HandleSelectedRowWidget);
		}
	}
}
