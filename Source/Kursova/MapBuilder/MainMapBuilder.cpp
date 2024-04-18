// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMapBuilder.h"

UMainMapBuilder::UMainMapBuilder()
{
	
}

void UMainMapBuilder::BuildFloor(const FVector2D& Dimentions)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	const int XStartSpawnPosition = CalculateStartingPoint(Dimentions.X);
	const int YStartSpawnPosition = CalculateStartingPoint(Dimentions.Y);

	int XSpawnPosition = XStartSpawnPosition;
	for (int i = 0; i < Dimentions.X; ++i, XSpawnPosition -= TileLength)
	{
		int YSpawnPosition = YStartSpawnPosition;
		for (int j = 0; j < Dimentions.Y; ++j, YSpawnPosition -= TileLength)
		{
			GetWorld()->SpawnActor<AFloor>(FloorClass, FVector(XSpawnPosition, YSpawnPosition, 0.f), FRotator::ZeroRotator, SpawnParameters);
		}
	}
}

void UMainMapBuilder::BuildGeneralWalls()
{
	
}

int UMainMapBuilder::CalculateStartingPoint(int Dimention)
{
		return ((Dimention - 1) * TileLength / 2) - (TileLength / 2);
}
