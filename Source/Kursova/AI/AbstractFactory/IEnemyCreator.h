// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kursova/AI/UEnemyInterface.h"
#include "UObject/Interface.h"
#include "IEnemyCreator.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIEnemyCreator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KURSOVA_API IIEnemyCreator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual IEnemyInterface* CreateBitingEnemies() = 0;
	virtual IEnemyInterface* CreateShootingEnemies() = 0;
	virtual IEnemyInterface* CreateExplodingEnemies() = 0;
};
