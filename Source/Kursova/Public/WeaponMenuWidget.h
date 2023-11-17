// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDataWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "WeaponMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UWeaponMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UVerticalBox* WeaponContent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponDataWidget> WeaponDataWidgetClass;
};

