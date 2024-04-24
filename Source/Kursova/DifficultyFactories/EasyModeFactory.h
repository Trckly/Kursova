// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModeFactory.h"
#include "FactoryProducts/EasyWeapon.h"
#include "EasyModeFactory.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KURSOVA_API UEasyModeFactory : public UObject, public IModeFactory
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AEasyWeapon> EasyWeaponClass;
	
public:
	virtual IWeaponInterface* CreateWeapon() override;
};
