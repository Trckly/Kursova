// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMapBuilder.h"
#include "BuilderProps/Obstacle.h"
#include "Kismet/KismetMathLibrary.h"

UForest* UMainMapBuilder::Forest = nullptr;

UMainMapBuilder::UMainMapBuilder()
{
	Forest = NewObject<UForest>();
}

void UMainMapBuilder::BuildFloor(const FVector2D& Dimensions)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	const int XStartSpawnPosition = CalculateStartingPoint(Dimensions.X);
	const int YStartSpawnPosition = CalculateStartingPoint(Dimensions.Y);

	int XSpawnPosition = XStartSpawnPosition;
	for (int i = 0; i < Dimensions.X; ++i, XSpawnPosition -= TileLength)
	{
		int YSpawnPosition = YStartSpawnPosition;
		for (int j = 0; j < Dimensions.Y; ++j, YSpawnPosition -= TileLength)
		{
			GetWorld()->SpawnActor<AFloor>(FloorClass, FVector(XSpawnPosition, YSpawnPosition, 0.f),
				FRotator::ZeroRotator, SpawnParameters);
		}
	}
}

void UMainMapBuilder::BuildGeneralWalls(const FVector2D& Dimensions)
{
	const float XStartSpawnPosition = CalculateStartingPoint(Dimensions.X) + TileLength;	// Offset for walls
	const float YStartSpawnPosition = CalculateStartingPoint(Dimensions.Y) + TileLength;	// Offset for walls
	FVector2D SpawnPosition(XStartSpawnPosition, YStartSpawnPosition);

	float RotationDegree = 180.f;
	int TileAmount;
	for ( int i = 0; i < 4; ++i, RotationDegree += 90.f)
	{
		if(RotationDegree == 0.f || RotationDegree == 180.f)
		{
			TileAmount = Dimensions.Y;
		}
		else
		{
			TileAmount = Dimensions.X;
		}
		SpawnPosition = WallLine(SpawnPosition, TileAmount, RotationDegree);
	}
}

void UMainMapBuilder::BuildObstacles(const FVector2D& Dimensions)
{
	int SpawnedObjectCounter = 0;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	while(SpawnedObjectCounter < Dimensions.X + Dimensions.Y)
	{
		auto SpawnedObject = GetWorld()->SpawnActor<AObstacle>(ObstacleClass, FVector(UKismetMathLibrary::RandomFloatInRange(-Dimensions.X/2.f * TileLength, Dimensions.X/2.f * TileLength),
			UKismetMathLibrary::RandomFloatInRange(-Dimensions.Y/2.f * TileLength, Dimensions.Y/2.f * TileLength), 1.f), FRotator::ZeroRotator, SpawnParameters);

		if(SpawnedObject)
		{
			SpawnedObjectCounter++;
		}
	}
}

void UMainMapBuilder::PlaceRadio(const FVector2D& Dimensions)
{
	Radio = ARadio::GetInstance(GetWorld(), RadioClass);
	
	if(Radio)
		Radio->SetActorLocation(FVector(UKismetMathLibrary::RandomFloatInRange(-Dimensions.X/2.f * TileLength, Dimensions.X/2.f * TileLength),
				UKismetMathLibrary::RandomFloatInRange(-Dimensions.Y/2.f * TileLength, Dimensions.Y/2.f * TileLength), 57.f), false,nullptr, ETeleportType::TeleportPhysics);
}

void UMainMapBuilder::CreateForest(const FVector2D& Dimensions)
{
	TArray<FString> TreeTypeNames = {"Jungle Tree", "Sakura"};
	for (int i = 0; i < Dimensions.X * Dimensions.Y / 2.f; ++i)
	{
		FVector Position = FVector(UKismetMathLibrary::RandomFloatInRange(-Dimensions.X/2.f * TileLength, Dimensions.X/2.f * TileLength),
				UKismetMathLibrary::RandomFloatInRange(-Dimensions.Y/2.f * TileLength, Dimensions.Y/2.f * TileLength), 0.f);

		FString TreeTypeName = TreeTypeNames[UKismetMathLibrary::RandomIntegerInRange(0, 1)];

		TSubclassOf<ATree> TreeClass;
		if(TreeTypeName == "Sakura")
		{
			TreeClass = SakuraTreeClass;
		}
		else
		{
			TreeClass = JungleTreeClass;
		}
		
		Forest->PlantTree(Position, TreeTypeName, TreeClass, GetWorld());
	}
}

int UMainMapBuilder::CalculateStartingPoint(int Dimension)
{
		return ((Dimension - 1) * TileLength / 2) - (TileLength / 2);
}

FVector2D UMainMapBuilder::WallLine(const FVector2D& StartingPoint, const int TileAmount, const float RotationDegrees)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	auto SpawnPosition = StartingPoint;

	float RotationRadians = RotationDegrees * PI / 180;
	FVector2D RotationDirection(FMath::Cos(RotationRadians), FMath::Sin(RotationRadians));
	RotationDirection.Normalize();
	for(int i = 0; i < TileAmount; ++i, SpawnPosition += RotationDirection * TileLength)
	{
		GetWorld()->SpawnActor<AWall>(WallClass, FVector(SpawnPosition.X,
			SpawnPosition.Y, 0.f), FRotator(0.f, RotationDegrees, 0.f), SpawnParameters);
	}

	return SpawnPosition;
}
