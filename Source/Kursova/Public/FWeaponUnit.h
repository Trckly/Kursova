// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FWeaponUnit.generated.h"

/**
 Holder for weapon main properties
 */
USTRUCT(BlueprintType)
struct FWeaponUnit : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Model;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MainType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Subtype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Capacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Manufacturer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Caliber;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Weight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Price;
};