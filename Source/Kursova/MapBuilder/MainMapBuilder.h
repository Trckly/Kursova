// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuilderProps/Floor.h"
#include "BuilderProps/Obstacle.h"
#include "BuilderProps/Wall.h"
#include "UObject/NoExportTypes.h"
#include "MainMapBuilder.generated.h"

/**
 * 
 */
enum EWallLineDirection
{
	Vertical,
	Horizontal
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AWall> WallClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AObstacle> ObstacleClass;

	void BuildFloor(const FVector2D& Dimensions);

	void BuildGeneralWalls(const FVector2D& Dimensions);

	void BuildObstacles(const FVector2D& Dimensions);

protected:
	int CalculateStartingPoint(int Dimension);

	FVector2D WallLine(const FVector2D& StartingPoint, const int TileAmount, const float RotationDegrees);
};
