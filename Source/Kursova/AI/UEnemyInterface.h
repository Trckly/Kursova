// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kursova/EnemyObserver/EnemyObserver.h"
#include "UObject/Interface.h"
#include "UEnemyInterface.generated.h"

/**
 * 
 */
UINTERFACE()
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class IEnemyInterface
{
	GENERATED_BODY()

	virtual void Die() = 0;
	
public:
	virtual int DealDamage() = 0;
	virtual void GetDamage(int Damage) = 0;
	virtual UBehaviorTree* GetBehaviourTree() = 0;

	virtual IEnemyInterface* Clone(FVector Location) = 0;
};
