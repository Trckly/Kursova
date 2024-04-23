// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonCreator.h"

#include "Kursova/Core/SessionGameMode.h"
#include "Math/UnrealMathUtility.h"

FVector USkeletonCreator::GetRandomLocation()
{
	const FVector2D XBorders = FVector2D(MapSize.X / 2.f - MapSize.X, MapSize.X - MapSize.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSize.Y / 2.f - MapSize.Y, MapSize.Y - MapSize.Y / 2.f);

	return FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f);
}

USkeletonCreator* USkeletonCreator::Create(ASessionGameMode* Owner, TSubclassOf<USkeletonCreator>const& SkeletonCreatorClass, const FVector2D& Map)
{
	auto NewInstance = NewObject<USkeletonCreator>(Owner, SkeletonCreatorClass);
	if(!NewInstance) return nullptr;
	
	NewInstance->MapSize = Map;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	AAIBitingEnemy* Biting = NewInstance->GetWorld()->SpawnActor<AAIBitingEnemy>(NewInstance->BitingSkeleton, FVector(8000.f, 8000.f, 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);

	if(!Biting) return nullptr;
	NewInstance->EnemyPrototypes.Add(EEnemyType::Biting, Biting);
	
	AAIExplodingEnemy* Exploding = NewInstance->GetWorld()->SpawnActor<AAIExplodingEnemy>(NewInstance->ExplodingSkeleton, FVector(8000.f, 8000.f, 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);

	if(!Exploding) return nullptr;
	NewInstance->EnemyPrototypes.Add(EEnemyType::Exploding, Exploding);
	
	return NewInstance;
}

IEnemyInterface* USkeletonCreator::CreateEnemies(EEnemyType EnemyType)
{
	return EnemyPrototypes[EnemyType]->Clone(GetRandomLocation());
}

IEnemyInterface* USkeletonCreator::CreateBitingEnemies(FVector2D MapSizeRed)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	const FVector2D XBorders = FVector2D(MapSizeRed.X / 2.f - MapSizeRed.X, MapSizeRed.X - MapSizeRed.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSizeRed.Y / 2.f - MapSizeRed.Y, MapSizeRed.Y - MapSizeRed.Y / 2.f);
	
	return GetWorld()->SpawnActor<AAIBitingEnemy>(BitingSkeleton, FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}

IEnemyInterface* USkeletonCreator::CreateShootingEnemies(FVector2D MapSizeRed)
{
	return nullptr;
}

IEnemyInterface* USkeletonCreator::CreateExplodingEnemies(FVector2D MapSizeRed)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	const FVector2D XBorders = FVector2D(MapSizeRed.X / 2.f - MapSizeRed.X, MapSizeRed.X - MapSizeRed.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSizeRed.Y / 2.f - MapSizeRed.Y, MapSizeRed.Y - MapSizeRed.Y / 2.f);
	
	return GetWorld()->SpawnActor<AAIExplodingEnemy>(ExplodingSkeleton, FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}
