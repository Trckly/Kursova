// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWeaponUnit.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WeaponDataWidget.generated.h"

// Maximum amount of symbols that
// can fit onto screen at the same time
constexpr int GString_Clipping_Len = 12;

// Declares delegate with one parameter
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRowClickedEvent, const FString&, ModelID);

/**
 Widget for hold output single weapon unit stats.
 Used in 'UWeaponMenuWidget' for further interaction
 */
UCLASS()
class KURSOVA_API UWeaponDataWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Widget constructor override
	virtual void NativeConstruct() override;
	
	///
	/// Link-variables for UI
	///
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ModelText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MainTypeText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SubtypeText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CapacityText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ManufacturerText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CaliberText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* LengthText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeightText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PriceText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* SelectionButton;

public:
	// Populates widget with weapon instance stats
	void InitWithData(const FWeaponUnit& WeaponUnit);

	// Cosmetic function for animating property
	// with exceeding amount of characters in it
	UFUNCTION(BlueprintImplementableEvent)
	void StartAnimation();

	///
	/// Getters
	///
	float GetCaliber();
	FString GetModelName();
	FString GetMainTypeName();
	FString GetSubtypeName();
	FString GetManufacturerName();

	// Returns all properties properties of a weapon
	TArray<FText> GetAllProperties();

	// Delegate, being called when selection button
	// was clicked. Handles row selection
	FOnRowClickedEvent OnRowClicked;

	//Executes OnRowClicked called
	UFUNCTION()
	void RowSelected();

	// Cosmetic. Sets button color to inactive
	UFUNCTION(BlueprintImplementableEvent)
	void SetButtonDisabledColor();

	// Cosmetic. Sets button color to active
	UFUNCTION(BlueprintImplementableEvent)
	void SetButtonEnabledColor();
};
