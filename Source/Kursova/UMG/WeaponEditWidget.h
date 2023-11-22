// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "WeaponEditWidget.generated.h"

// Declares delegate with two parameters
DECLARE_DYNAMIC_DELEGATE_TwoParams(FAcceptedDelegate, const TArray<FText>&, ChangedValues, const FString&, OriginalModelName);


/**
 Properties editor widget 
 */
UCLASS()
class KURSOVA_API UWeaponEditWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Widget constructor override
virtual void NativeConstruct() override;

	///
	/// Link-variables for UI
	///
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* ModelEditBox;

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* MainTypeEditBox;

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* SubtypeEditBox;

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* CapacityEditBox;

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* ManufacturerEditBox;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* CaliberEditBox;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* LengthEditBox;

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* WeightEditBox;

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* PriceEditBox;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ProceedButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* CancelButton;

	// In case of changing model name is used
	// to identify current object to change
	UPROPERTY()
	FString PreviousModelName;

public:
	// Setups input text boxes in order to see
	// what changes are you making and to prevent
	// writing all other properties by hand, if
	// you don't want them to change
	void SetupInputBoxes(const TArray<FText>& PropertiesArray);

	// Calls delegate to override values in
	// weapon structure and parent widget
	UFUNCTION()
	void Proceed() noexcept;

	// Destroys widget, discards all changes
	UFUNCTION()
	void Cancel() noexcept;

	// Delegate variable,
	// executes on proceed button pressed
	FAcceptedDelegate OnAcceptedEvent;
};
