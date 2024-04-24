// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModeFactory.h"
#include "FactoryProducts/HardWeapon.h"
#include "UObject/NoExportTypes.h"
#include "HardModeFactory.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KURSOVA_API UHardModeFactory : public UObject, public IModeFactory
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AHardWeapon> HardWeaponClass;
	
public:
	virtual IWeaponInterface* CreateWeapon() override;
};
