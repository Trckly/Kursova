// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonCreator.h"

IEnemyInterface* USkeletonCreator::CreateBitingEnemies()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	return GetWorld()->SpawnActor<AAIBitingEnemy>(BitingSkeleton, FVector(0.f, 0.f, 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}

IEnemyInterface* USkeletonCreator::CreateShootingEnemies()
{
	return nullptr;
}

IEnemyInterface* USkeletonCreator::CreateExplodingEnemies()
{
	return nullptr;
}
