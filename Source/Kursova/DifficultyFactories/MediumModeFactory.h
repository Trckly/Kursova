// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModeFactory.h"
#include "FactoryProducts/MediumWeapon.h"
#include "MediumModeFactory.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KURSOVA_API UMediumModeFactory : public UObject, public IModeFactory
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AMediumWeapon> MediumWeaponClass;
	
public:
	virtual IWeaponInterface* CreateWeapon() override;
};
