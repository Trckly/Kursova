// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeType.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class KURSOVA_API UTreeType : public UObject
{
	GENERATED_BODY()

protected:
	FString TreeTypeName;

	UPROPERTY()
	TSubclassOf<UObject> TreeClass;

public:
	UTreeType();
	static UTreeType* CreateTreeType(const FString& Name, TSubclassOf<UObject> Class);
	void Initialize(const FString& Name, TSubclassOf<UObject> Class);

	TSubclassOf<UObject> GetTreeClass();
	FString GetName();
};
