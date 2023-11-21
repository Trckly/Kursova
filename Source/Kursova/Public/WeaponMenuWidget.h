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
 * 
 */
UCLASS()
class KURSOVA_API UWeaponMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	enum EComboBoxValues
	{
		ModelFilter,
		MainTypeFilter,
		SubtypeFilter,
		ManufacturerFilter
	};
	
	virtual void NativeConstruct() override;

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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponDataWidget> WeaponDataWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponEditWidget> WeaponEditWidgetClass;

	UPROPERTY()
	TArray<UWeaponDataWidget*> ChildRowWidgets;

	UPROPERTY()
	UWeaponDataWidget* SelectedWidget;

	UPROPERTY()
	UWeaponEditWidget* EditWidget;

	UPROPERTY()
	TArray<AWeaponClass*> ActorPickedWeapons;

	bool bSortFlipFlop;

public:
	virtual void RemoveFromParent() override;
	
	UFUNCTION()
	void SortByCaliber();

	UFUNCTION()
	void CreatePropertiesEditor();

	UFUNCTION()
	void EditWeaponUnitProperties(const TArray<FText>& ChangedValues, const FString& OriginalModelName);

	UFUNCTION()
	void HandleSelectedRowWidget(const FString& ModelName);

	UFUNCTION()
	void FilterFunc(const FText& TypedText);

	void ShellSort(TArray<UWeaponDataWidget*>& Children, bool& bAscending);

	void SetActorPickedWeapons(const TArray<AWeaponClass*>& PickedWeapons);

	void FilterByModel(const FText& NewModelName);

	void FilterByMainType(const FText& NewMainTypeName);

	void FilterBySubtype(const FText& NewSubtypeName);

	void FilterByManufacturer(const FText& NewManufacturerName);

	UWeaponDataWidget* GetSelectedWidget();
};

