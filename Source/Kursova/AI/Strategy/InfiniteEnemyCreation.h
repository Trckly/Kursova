// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCreationMode.h"
#include "UObject/NoExportTypes.h"
#include "InfiniteEnemyCreation.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UInfiniteEnemyCreation : public UObject, public IEnemyCreationMode
{
	GENERATED_BODY()

	FTimerHandle TimerHandle;
	
public:
	virtual void CreateEnemies(IIEnemyCreator* EnemyCreator, TArray<IEnemyInterface*>* Enemies) override;

	void Create(IIEnemyCreator* EnemyCreator, TArray<IEnemyInterface*>* Enemies);
};