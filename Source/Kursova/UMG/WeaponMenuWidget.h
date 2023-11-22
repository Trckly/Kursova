// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeaponClass.h"
#include "WeaponDataWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/VerticalBox.h"
#include "Kursova/UMG/WeaponEditWidget.h"
#include "WeaponMenuWidget.generated.h"

/**
 General widget class.
 Acts like a manager for all user functions
 */
UCLASS()
class KURSOVA_API UWeaponMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Defines type of filtering
	enum EComboBoxValues
	{
		ModelFilter,
		MainTypeFilter,
		SubtypeFilter,
		ManufacturerFilter
	};

	// Overrode widget constructor
	virtual void NativeConstruct() override;

	///
	/// Link-variables for UI
	///
	/// 
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* WeaponContent;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* SortButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* EditButton;

	UPROPERTY(meta=(BindWidget))
	UComboBoxString* FilterComboBox;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* FilterTextBox;

	// Gets blueprint UWeaponDataWidget child class
	// for creating widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponDataWidget> WeaponDataWidgetClass;

	// Gets blueprint UWeaponEditWidget child class
	// for creating widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponEditWidget> WeaponEditWidgetClass;

	// Stores pointers to all instances of UWeaponDataWidget created widgets
	UPROPERTY()
	TArray<UWeaponDataWidget*> ChildRowWidgets;

	// Stores pointer to selected row
	UPROPERTY()
	UWeaponDataWidget* SelectedWidget;

	// Stores pointer to created UWeaponEditWidget
	UPROPERTY()
	UWeaponEditWidget* EditWidget;

	// Stores pointer to all weapon instances picked by player
	UPROPERTY()
	TArray<AWeaponClass*> ActorPickedWeapons;

	// Makes sort function work in ascending and
	// descending order by repeatedly clicking button
	bool bSortFlipFlop;

public:
	// Overrode Widget destruction method.
	// Destroys Properties editor if it is opened
	virtual void RemoveFromParent() override;

	// Bound to SortButton. Performs sorting
	UFUNCTION()
	void SortByCaliber();

	// Creating widget for weapon properties edit
	UFUNCTION()
	void CreatePropertiesEditor();

	// Delegate function. Edits weapon properties values
	UFUNCTION()
	void EditWeaponUnitProperties(const TArray<FText>& ChangedValues, const FString& OriginalModelName);

	// Cosmetic. Visually describes selected row
	UFUNCTION()
	void HandleSelectedRowWidget(const FString& ModelName);

	// Filters weapons by properties available in EComboBoxValues
	UFUNCTION()
	void FilterFunc(const FText& TypedText);

	// Shell's sorting algorithm remade for widgets
	void ShellSort(TArray<UWeaponDataWidget*>& Children, bool& bAscending);

	// Writes player's picked weapon to further work with them
	void SetActorPickedWeapons(const TArray<AWeaponClass*>& PickedWeapons);

	// Filter algorithm (by model)
	void FilterByModel(const FText& NewModelName);

	// Filter algorithm (by main type)
	void FilterByMainType(const FText& NewMainTypeName);

	// Filter algorithm (by subtype)
	void FilterBySubtype(const FText& NewSubtypeName);

	// Filter algorithm (by manufacturer)
	void FilterByManufacturer(const FText& NewManufacturerName);

	// Getter for active/selected widget
	UWeaponDataWidget* GetSelectedWidget();
};

