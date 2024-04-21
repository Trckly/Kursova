// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinCreator.h"

#include "Kursova/AI/AIBitingEnemy.h"

IEnemyInterface* UGoblinCreator::CreateBitingEnemies(FVector2D MapSize)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	const FVector2D XBorders = FVector2D(MapSize.X / 2.f - MapSize.X, MapSize.X - MapSize.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSize.Y / 2.f - MapSize.Y, MapSize.Y - MapSize.Y / 2.f);
	
	return GetWorld()->SpawnActor<AAIBitingEnemy>(BitingGoblin, FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}

IEnemyInterface* UGoblinCreator::CreateShootingEnemies(FVector2D MapSize)
{
	return nullptr;
}

IEnemyInterface* UGoblinCreator::CreateExplodingEnemies(FVector2D MapSize)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	const FVector2D XBorders = FVector2D(MapSize.X / 2.f - MapSize.X, MapSize.X - MapSize.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSize.Y / 2.f - MapSize.Y, MapSize.Y - MapSize.Y / 2.f);
	
	return GetWorld()->SpawnActor<AAIExplodingEnemy>(ExplodingGoblin, FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}
