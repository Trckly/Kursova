// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameMode.h"

void ASessionGameMode::CreateEnemies(const TScriptInterface<IIEnemyCreator>& EnemyCreator)
{
	IEnemyInterface* NewEnemy = EnemyCreator->CreateBitingEnemies();
	if(NewEnemy)
		Enemies.Add(NewEnemy);
}

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
			MainMapBuilder->PlaceRadio(GeneralUniversalDimensions);
		}
	}
}

void ASessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	
	if(MapName == FString("GoblinsMap"))
	{
		if(GoblinCreatorClass)
		{
			GoblinCreator = NewObject<UGoblinCreator>(this, GoblinCreatorClass);

			if(GoblinCreator)
			{
				CreateEnemies(GoblinCreator);
			}
		}
	}else if(MapName == FString("SkeletonsMap"))
	{
		if(SkeletonCreatorClass)
		{
			SkeletonCreator = NewObject<USkeletonCreator>(this, SkeletonCreatorClass);

			if(SkeletonCreator)
			{
				CreateEnemies(SkeletonCreator);
			}
		}
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create enemies!"));
	}
}