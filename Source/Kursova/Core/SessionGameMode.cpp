// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameMode.h"

void ASessionGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(GoblinCreatorClass)
	{
		GoblinCreator = NewObject<UGoblinCreator>(this, GoblinCreatorClass);

		if(GoblinCreator)
		{
			IEnemyInterface* NewEnemy = GoblinCreator->CreateBitingEnemies();
			if(NewEnemy)
				Enemies.Add(NewEnemy);
		}
	}
	
}
