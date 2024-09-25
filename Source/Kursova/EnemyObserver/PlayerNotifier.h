// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kursova/AI/UEnemyInterface.h"
#include "UObject/Interface.h"
#include "PlayerNotifier.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerNotifier : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KURSOVA_API IPlayerNotifier
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Attach(IEnemyObserver* EnemyInterface) = 0;
	virtual void Detach(IEnemyObserver* EnemyInterface) = 0;
	virtual void Notify() = 0;
};
