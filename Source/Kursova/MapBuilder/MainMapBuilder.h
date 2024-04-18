// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuilderProps/Floor.h"
#include "UObject/NoExportTypes.h"
#include "MainMapBuilder.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KURSOVA_API UMainMapBuilder : public UObject
{
	GENERATED_BODY()
protected:
	const int TileLength = 400;
	
public:
	UMainMapBuilder();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AFloor> FloorClass;

	void BuildFloor(const FVector2D& Dimentions);

	void BuildGeneralWalls();

protected:
	int CalculateStartingPoint(int Dimention);
};
