// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameMode.h"

void ASessionGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(MainMapBuilderClass)
	{
		MainMapBuilder = NewObject<UMainMapBuilder>(this, MainMapBuilderClass);

		if(MainMapBuilder)
		{
			FVector2D GeneralUniversalDimensions(10.f, 10.f);
			MainMapBuilder->BuildFloor(GeneralUniversalDimensions);
			MainMapBuilder->BuildGeneralWalls(GeneralUniversalDimensions);
			MainMapBuilder->BuildObstacles(GeneralUniversalDimensions);
		}
	}
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