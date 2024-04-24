// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FactoryProducts/WeaponInterface.h"
#include "UObject/Interface.h"
#include "ModeFactory.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UModeFactory : public UInterface
{
	GENERATED_BODY()
};

// UENUM()
// enum EDifficultyMode
// {
// 	Easy = 0,
// 	Medium,
// 	Hard
// };

/**
 * 
 */
class KURSOVA_API IModeFactory
{
	GENERATED_BODY()

public:
	virtual IWeaponInterface* CreateWeapon() = 0;
};
