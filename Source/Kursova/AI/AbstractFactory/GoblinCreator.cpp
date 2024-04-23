// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinCreator.h"

#include "Kursova/AI/AIBitingEnemy.h"
#include "Kursova/Core/SessionGameMode.h"


FVector UGoblinCreator::GetRandomLocation()
{
	const FVector2D XBorders = FVector2D(MapSize.X / 2.f - MapSize.X, MapSize.X - MapSize.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSize.Y / 2.f - MapSize.Y, MapSize.Y - MapSize.Y / 2.f);

	return FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f);
}

UGoblinCreator* UGoblinCreator::Create(ASessionGameMode* Owner, TSubclassOf<UGoblinCreator>const& GoblinCreatorClass, const FVector2D& Map)
{
	auto NewInstance = NewObject<UGoblinCreator>(Owner, GoblinCreatorClass);
	if(!NewInstance) return nullptr;
	
	NewInstance->MapSize = Map;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	AAIBitingEnemy* Biting = NewInstance->GetWorld()->SpawnActor<AAIBitingEnemy>(NewInstance->BitingGoblin, FVector(8000.f, 8000.f, 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);

	if(!Biting) return nullptr;
	NewInstance->EnemyPrototypes.Add(EEnemyType::Biting, Biting);
	
	AAIExplodingEnemy* Exploding = NewInstance->GetWorld()->SpawnActor<AAIExplodingEnemy>(NewInstance->ExplodingGoblin, FVector(8000.f, 8000.f, 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);

	if(!Exploding) return nullptr;
	NewInstance->EnemyPrototypes.Add(EEnemyType::Exploding, Exploding);
	
	return NewInstance;
}

IEnemyInterface* UGoblinCreator::CreateEnemies(EEnemyType EnemyType)
{
	return EnemyPrototypes[EnemyType]->Clone(GetRandomLocation());
}

IEnemyInterface* UGoblinCreator::CreateBitingEnemies(FVector2D MapSizeRed)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	const FVector2D XBorders = FVector2D(MapSizeRed.X / 2.f - MapSizeRed.X, MapSizeRed.X - MapSizeRed.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSizeRed.Y / 2.f - MapSizeRed.Y, MapSizeRed.Y - MapSizeRed.Y / 2.f);

	return GetWorld()->SpawnActor<AAIBitingEnemy>(BitingGoblin, FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}

IEnemyInterface* UGoblinCreator::CreateShootingEnemies(FVector2D MapSizeRed)
{
	return nullptr;
}

IEnemyInterface* UGoblinCreator::CreateExplodingEnemies(FVector2D MapSizeRed)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	const FVector2D XBorders = FVector2D(MapSizeRed.X / 2.f - MapSizeRed.X, MapSizeRed.X - MapSizeRed.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSizeRed.Y / 2.f - MapSizeRed.Y, MapSizeRed.Y - MapSizeRed.Y / 2.f);
	
	return GetWorld()->SpawnActor<AAIExplodingEnemy>(ExplodingGoblin, FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}
