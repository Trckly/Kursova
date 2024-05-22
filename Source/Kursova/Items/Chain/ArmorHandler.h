// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractHandler.h"
#include "ArmorHandler.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FAddArmor, float, ArmorPoints);

UCLASS()
class KURSOVA_API UArmorHandler : public UAbstractHandler
{
	GENERATED_BODY()

public:
	virtual bool Handle(AAbstractItem* PickUpItem) override;

	FAddArmor AddArmor;
};
