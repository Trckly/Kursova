// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameMode.h"

void ASessionGameMode::CreateEnemies(const TScriptInterface<IIEnemyCreator>& EnemyCreator)
{
	while(NumberOfBiters--)
	{
		IEnemyInterface* NewEnemy = EnemyCreator->CreateBitingEnemies(GeneralUniversalDimensions * 400.f);
		if(NewEnemy)
			Enemies.Add(NewEnemy);
		else
			NumberOfBiters++;
	}
	while(NumberOfSuicidal--)
	{
		IEnemyInterface* NewEnemy = EnemyCreator->CreateExplodingEnemies(GeneralUniversalDimensions * 400.f);
		if(NewEnemy)
			Enemies.Add(NewEnemy);
		else
			NumberOfSuicidal++;		
	}
}

void ASessionGameMode::CreateHardPoint()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	do
	{
		CoolItem = ACoolItem::GetInstance(GetWorld(), CoolItemClass, GeneralUniversalDimensions * 400.f, SpawnParameters);
	}while(!CoolItem);

	if(CoolItem)
		CoolItem->ScoreNotify.BindDynamic(this, &ASessionGameMode::HandleHardPointCapture);	
}

void ASessionGameMode::HandleHardPointCapture()
{
	CoolItem->ScoreNotify.Clear();
	CoolItem = nullptr;
	ACoolItem::ClearInstance();
	CreateHardPoint();
}


void ASessionGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(MainMapBuilderClass)
	{
		MainMapBuilder = NewObject<UMainMapBuilder>(this, MainMapBuilderClass);

		if(MainMapBuilder)
		{
			MainMapBuilder->BuildFloor(GeneralUniversalDimensions);
			MainMapBuilder->BuildGeneralWalls(GeneralUniversalDimensions);
			MainMapBuilder->BuildObstacles(GeneralUniversalDimensions);
		}
	}
	
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

	if(CoolItemClass)
	{
		CreateHardPoint();
	}
}

void ASessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}