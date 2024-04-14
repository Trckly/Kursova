// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinCreator.h"

#include "Kursova/AI/AIBitingEnemy.h"

IEnemyInterface* UGoblinCreator::CreateBitingEnemies()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	return GetWorld()->SpawnActor<AAIBitingEnemy>(BitingGoblin, FVector(0.f, 0.f, 92.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
}

IEnemyInterface* UGoblinCreator::CreateShootingEnemies()
{
	return nullptr;
}

IEnemyInterface* UGoblinCreator::CreateExplodingEnemies()
{
	return nullptr;
}
