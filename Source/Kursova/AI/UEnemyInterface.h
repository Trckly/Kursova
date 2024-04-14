// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
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

public:
	virtual int DealDamage() = 0;
	virtual int TakeDamage(int Damage) = 0;

	virtual UBehaviorTree* GetBehaviourTree() = 0;
};
