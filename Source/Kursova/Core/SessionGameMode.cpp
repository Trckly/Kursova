// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameMode.h"

void ASessionGameMode::CreateEnemies(IIEnemyCreator* EnemyCreator)
{
	while(NumberOfBiters--)
	{
		IEnemyInterface* NewEnemy = EnemyCreator->CreateEnemies(EEnemyType::Biting);
		if(NewEnemy)
			Enemies.Add(NewEnemy);
		else
			NumberOfBiters++;
	}
	while(NumberOfSuicidal--)
	{
		IEnemyInterface* NewEnemy = EnemyCreator->CreateEnemies(EEnemyType::Exploding);
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
			MainMapBuilder->PlaceRadio(GeneralUniversalDimensions);
		}
	}
	
	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	
	if(MapName == FString("GoblinsMap"))
	{
		if(GoblinCreatorClass)
		{
			GoblinCreator = UGoblinCreator::Create(this, GoblinCreatorClass, GeneralUniversalDimensions * 400.f);

			if(GoblinCreator)
			{
				CreateEnemies(GoblinCreator);
			}
		}
	}else if(MapName == FString("SkeletonsMap"))
	{
		if(SkeletonCreatorClass)
		{
			SkeletonCreator = USkeletonCreator::Create(this, SkeletonCreatorClass, GeneralUniversalDimensions * 400.f);

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