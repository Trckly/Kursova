// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tree.h"
#include "TreeFactory.h"
#include "UObject/NoExportTypes.h"
#include "Forest.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class KURSOVA_API UForest : public UObject
{
	GENERATED_BODY()
public:
	UForest();

protected:
	UPROPERTY()
	TArray<ATree*> Trees;

	UPROPERTY()
	UTreeFactory* TreeFactory;
	
public:
	void PlantTree(FVector Position, const FString& TreeTypeName, TSubclassOf<ATree> TreeClass, UWorld* World);
};
