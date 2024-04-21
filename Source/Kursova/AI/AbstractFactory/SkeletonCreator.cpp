// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonCreator.h"
#include "Math/UnrealMathUtility.h"

IEnemyInterface* USkeletonCreator::CreateBitingEnemies(FVector2D MapSize)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	const FVector2D XBorders = FVector2D(MapSize.X / 2.f - MapSize.X, MapSize.X - MapSize.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSize.Y / 2.f - MapSize.Y, MapSize.Y - MapSize.Y / 2.f);
	
	return GetWorld()->SpawnActor<AAIBitingEnemy>(BitingSkeleton, FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}

IEnemyInterface* USkeletonCreator::CreateShootingEnemies(FVector2D MapSize)
{
	return nullptr;
}

IEnemyInterface* USkeletonCreator::CreateExplodingEnemies(FVector2D MapSize)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	const FVector2D XBorders = FVector2D(MapSize.X / 2.f - MapSize.X, MapSize.X - MapSize.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSize.Y / 2.f - MapSize.Y, MapSize.Y - MapSize.Y / 2.f);
	
	return GetWorld()->SpawnActor<AAIExplodingEnemy>(ExplodingSkeleton, FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}
