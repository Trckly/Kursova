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

UENUM()
enum class EEnemyType
{
	Biting = 0,
	Shooting,
	Exploding
};
/**
 * 
 */
class KURSOVA_API IIEnemyCreator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual IEnemyInterface* CreateEnemies(EEnemyType EnemyType) = 0;
	
	virtual IEnemyInterface* CreateBitingEnemies(FVector2D MapSize) = 0;
	virtual IEnemyInterface* CreateShootingEnemies(FVector2D MapSize) = 0;
	virtual IEnemyInterface* CreateExplodingEnemies(FVector2D MapSize) = 0;
};
