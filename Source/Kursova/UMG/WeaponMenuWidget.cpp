// Fill out your copyright notice in the Description page of Project Settings.


#include "Kursova/UMG/WeaponMenuWidget.h"

void UWeaponMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SortButton->OnClicked.AddDynamic(this, &UWeaponMenuWidget::SortByCaliber);
	EditButton->OnClicked.AddDynamic(this, &UWeaponMenuWidget::CreatePropertiesEditor);
	FilterTextBox->OnTextChanged.AddDynamic(this, &UWeaponMenuWidget::FilterFunc);

	bSortFlipFlop = true;
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
	
	ShellSort(Children, bSortFlipFlop);
	for(auto Child : Children)
	{
		WeaponContent->AddChild(Child);
	}
}

void UWeaponMenuWidget::CreatePropertiesEditor()
{
	if(SelectedWidget)
	{
		TArray<FText> Test = {FText::FromString(TEXT("fdas")), FText::FromString(TEXT("ffdss"))};
		
		UWeaponEditWidget* CreatedWidget = CreateWidget<UWeaponEditWidget>(this, WeaponEditWidgetClass);
		if(CreatedWidget)
		{
			EditWidget = CreatedWidget;
			EditWidget->OnAcceptedEvent.BindDynamic(this, & UWeaponMenuWidget::EditWeaponUnitProperties);
			
			EditWidget->SetupInputBoxes(SelectedWidget->GetAllProperties());
			// EditWidget->SetupInputBoxes(Test);
				
			// try
			// {
			// 	std::ifstream Fin("Test.txt");
			// 	Fin >> *ActorPickedWeapons[0];
			//  Fin.close();
			// }
			// catch(ExceptionWeaponOutput Exception)
			// {
			// 	const FString Exc(UTF8_TO_TCHAR(Exception.what()));
			// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *Exc);
			// }
			EditWidget->AddToViewport();
			
		}
	}
}

void UWeaponMenuWidget::EditWeaponUnitProperties(const TArray<FText>& ChangedValues, const FString& OriginalModelName)
{
	for (int i = 0; i < ActorPickedWeapons.Num(); ++i)
	{
		if(ActorPickedWeapons[i] && ActorPickedWeapons[i]->GetStructure().Model == OriginalModelName)
		{
			ActorPickedWeapons[i]->EditStructure(ChangedValues);
		}
	}

	WeaponContent->ClearChildren();
	ChildRowWidgets.Empty();
	for (auto WeaponInstance : ActorPickedWeapons)
	{
		auto WidgetCreated = CreateWidget<UWeaponDataWidget>(this, WeaponDataWidgetClass);
		if(WidgetCreated && WeaponInstance)
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

void UWeaponMenuWidget::FilterFunc(const FText& TypedText)
{
	if(TypedText.ToString().Len() == 0)
	{
		WeaponContent->ClearChildren();
		for(auto Child : ChildRowWidgets)
		{
			WeaponContent->AddChild(Child);
		}
	}
	else
	{
		int FilterType = FilterComboBox->GetSelectedIndex();

		switch (FilterType)
		{
		case ModelFilter:
			FilterByModel(TypedText);
			break;

		case MainTypeFilter:
			FilterByMainType(TypedText);
			break;
			
		case SubtypeFilter:
			FilterBySubtype(TypedText);
			break;

		case ManufacturerFilter:
			FilterByManufacturer(TypedText);
			break;

		default:
			break;
		}
	}
}

void UWeaponMenuWidget::ShellSort(TArray<UWeaponDataWidget*>& Children, bool& bAscending)
{
	if(Children.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot sort empty array!"));
		return;
	}
	if(bAscending)
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
	else
	{
		for (int Gap = Children.Num() / 2; Gap > 0; Gap /= 2)
		{
			for (int i = Gap; i < Children.Num(); ++i)
			{
				auto Temp = Children[i];

				int j;
				for(j = i; j >= Gap && Children[j-Gap]->GetCaliber() < Temp->GetCaliber(); j -= Gap)
					Children[j] = Children[j-Gap];
			
				Children[j] = Temp;
			}
		}
	}
	bAscending = !bAscending;
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
		if(WidgetCreated && WeaponInstance)
		{
			WidgetCreated->InitWithData(WeaponInstance->GetStructure());
			WeaponContent->AddChild(WidgetCreated);
				
			ChildRowWidgets.Push(WidgetCreated);
			WidgetCreated->OnRowClicked.BindDynamic(this, &UWeaponMenuWidget::HandleSelectedRowWidget);
		}
	}
}

void UWeaponMenuWidget::FilterByModel(const FText& NewModelName)
{
	WeaponContent->ClearChildren();

	for(auto Child : ChildRowWidgets)
	{
		FString ModelName = Child->GetModelName();
		FString NoSpacesModelName = ModelName;
		NoSpacesModelName.RemoveSpacesInline();
		if(ModelName.Contains(NewModelName.ToString()) ||
			NoSpacesModelName.Contains(NewModelName.ToString()))
		{
			WeaponContent->AddChild(Child);
		}
	}
}

void UWeaponMenuWidget::FilterByMainType(const FText& NewMainTypeName)
{
	WeaponContent->ClearChildren();

	for(auto Child : ChildRowWidgets)
	{
		FString MainTypeName = Child->GetMainTypeName();
		FString NoSpacesMainTypeName = MainTypeName;
		NoSpacesMainTypeName.RemoveSpacesInline();
		if(MainTypeName.Contains(NewMainTypeName.ToString()) ||
			NoSpacesMainTypeName.Contains(NewMainTypeName.ToString()))
		{
			WeaponContent->AddChild(Child);
		}
	}
}

void UWeaponMenuWidget::FilterBySubtype(const FText& NewSubtypeName)
{
	WeaponContent->ClearChildren();

	for(auto Child : ChildRowWidgets)
	{
		FString SubtypeName = Child->GetSubtypeName();
		FString NoSpacesSubtypeName = SubtypeName;
		NoSpacesSubtypeName.RemoveSpacesInline();
		if(SubtypeName.Contains(NewSubtypeName.ToString()) ||
			NoSpacesSubtypeName.Contains(NewSubtypeName.ToString()))
		{
			WeaponContent->AddChild(Child);
		}
	}
}

void UWeaponMenuWidget::FilterByManufacturer(const FText& NewManufacturerName)
{
	WeaponContent->ClearChildren();

	for(auto Child : ChildRowWidgets)
	{
		FString ManufacturerName = Child->GetManufacturerName();
		FString NoSpacesManufacturerName = ManufacturerName;
		NoSpacesManufacturerName.RemoveSpacesInline();
		if(ManufacturerName.Contains(NewManufacturerName.ToString()) ||
			NoSpacesManufacturerName.Contains(NewManufacturerName.ToString()))
		{
			WeaponContent->AddChild(Child);
		}
	}
}

UWeaponDataWidget* UWeaponMenuWidget::GetSelectedWidget()
{
	return SelectedWidget;
}
