// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponClass.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
};
