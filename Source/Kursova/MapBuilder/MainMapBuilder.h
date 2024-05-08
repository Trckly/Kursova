// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuilderProps/Floor.h"
#include "BuilderProps/Obstacle.h"
#include "BuilderProps/Wall.h"
#include "Kursova/ForestFlyweight/Forest.h"
#include "Kursova/Singletones/Radio.h"
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

private:
	UMainMapBuilder();
	
protected:
	
	const int TileLength = 400;

	UPROPERTY()
	UMainMapBuilder* SelfInstance;

	UPROPERTY()
	ARadio* Radio;

	static UForest* Forest;
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AFloor> FloorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AWall> WallClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AObstacle> ObstacleClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ARadio> RadioClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ATree> JungleTreeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ATree> SakuraTreeClass;
	
	void BuildFloor(const FVector2D& Dimensions);

	void BuildGeneralWalls(const FVector2D& Dimensions);

	void BuildObstacles(const FVector2D& Dimensions);

	void PlaceRadio(const FVector2D& Dimensions);

	void CreateForest(const FVector2D& Dimensions);

protected:
	int CalculateStartingPoint(int Dimension);

	FVector2D WallLine(const FVector2D& StartingPoint, const int TileAmount, const float RotationDegrees);
};
