// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeType.h"
#include "TreeFactory.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class KURSOVA_API UTreeFactory : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<UTreeType*> TreeTypes;

public:
	UTreeType* GetTreeType(const FString& Name, TSubclassOf<UObject> TreeClass);
};
