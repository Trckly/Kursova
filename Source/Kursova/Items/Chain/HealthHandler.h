// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractHandler.h"
#include "HealthHandler.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FAddHealth, float, HealthPoints);

UCLASS()
class KURSOVA_API UHealthHandler : public UAbstractHandler
{
	GENERATED_BODY()
public:
	virtual bool Handle(AAbstractItem* PickUpItem) override;

	FAddHealth AddHealth;
};
