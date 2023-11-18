// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "WeaponEditWidget.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UWeaponEditWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
virtual void NativeConstruct() override;
	
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

	UPROPERTY(meta=(BindWidget))
	UButton* ProceedButton;

	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;

public:
	void SetupInputBoxes();

	UFUNCTION()
	void Proceed();

	UFUNCTION()
	void Cancel();
};
