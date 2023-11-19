// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWeaponUnit.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WeaponDataWidget.generated.h"

/**
 * 
 */
constexpr int GString_Clipping_Len = 12;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRowClickedEvent, const FString&, ModelID);

UCLASS()
class KURSOVA_API UWeaponDataWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
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
	void InitWithData(const FWeaponUnit& WeaponUnit);

	UFUNCTION(BlueprintImplementableEvent)
	void StartAnimation();

	float GetCaliber();

	FString GetModelName();

	TArray<FText> GetAllProperties();

	FOnRowClickedEvent OnRowClicked;

	UFUNCTION()
	void RowSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void SetButtonDisabledColor();

	UFUNCTION(BlueprintImplementableEvent)
	void SetButtonEnabledColor();
};
