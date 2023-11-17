// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWeaponUnit.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WeaponDataWidget.generated.h"

/**
 * 
 */
constexpr int StringClippingLength = 12;

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

public:
	void InitWithData(const FWeaponUnit& WeaponUnit);

	UFUNCTION(BlueprintImplementableEvent)
	void StartAnimation();
};
