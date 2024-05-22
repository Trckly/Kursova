// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCreationMode.h"
#include "UObject/NoExportTypes.h"
#include "FiniteEnemyCreation.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UFiniteEnemyCreation : public UObject, public IEnemyCreationMode
{
	GENERATED_BODY()

	int NumberOfBiters = 10;
	int NumberOfSuicidal = 10;
	
public:
	
	virtual void CreateEnemies(IIEnemyCreator* EnemyCreator, TArray<IEnemyInterface*>* Enemies) override;
};
