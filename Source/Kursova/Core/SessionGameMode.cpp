// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameMode.h"

#include "MainPlayer.h"
#include "Kursova/DifficultyFactories/EasyModeFactory.h"
#include "Kursova/DifficultyFactories/HardModeFactory.h"
#include "Kursova/DifficultyFactories/MediumModeFactory.h"

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
			MainMapBuilder->CreateForest(GeneralUniversalDimensions);
			// MainMapBuilder->BuildGeneralWalls(GeneralUniversalDimensions);
			// MainMapBuilder->BuildObstacles(GeneralUniversalDimensions);
			// MainMapBuilder->PlaceRadio(GeneralUniversalDimensions);
		}
	}

	AMainPlayer* MainPlayer = Cast<AMainPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
	if(MainPlayer)
		MainPlayer->SetDifficultyModeDelegate.BindDynamic(this, &ASessionGameMode::SetDifficultyMode);
	else
		UE_LOG(LogGameMode, Error, TEXT("Failed to get MainPlayer reference"));
	
	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	
	if(MapName == FString("GoblinsMap"))
	{
		if(GoblinCreatorClass)
		{
			GoblinCreator = UGoblinCreator::Create(this, GoblinCreatorClass, GeneralUniversalDimensions * 400.f);

			if(GoblinCreator)
				CreateEnemies(GoblinCreator);
		}
	}else if(MapName == FString("SkeletonsMap"))
	{
		if(SkeletonCreatorClass)
		{
			SkeletonCreator = USkeletonCreator::Create(this, SkeletonCreatorClass, GeneralUniversalDimensions * 400.f);

			if(SkeletonCreator)
				CreateEnemies(SkeletonCreator);
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to create enemies!"));

	if(CoolItemClass)
	{
		CreateHardPoint();
	}
}

TScriptInterface<IModeFactory> ASessionGameMode::SetDifficultyMode(const FString& Difficulty)
{
	if(DifficultyMode == "")
		DifficultyMode = Difficulty;

	if(DifficultyMode == "Easy")
		ModeFactory = NewObject<UEasyModeFactory>(this, EasyModeFactoryClass, TEXT("Easy Mode Factory"));
	if(DifficultyMode == "Medium")
		ModeFactory = NewObject<UMediumModeFactory>(this, MediumModeFactoryClass, TEXT("Medium Mode Factory"));
	if(DifficultyMode == "Hard")
		ModeFactory = NewObject<UHardModeFactory>(this, HardModeFactoryClass, TEXT("Hard Mode Factory"));

	if(!ModeFactory)
		UE_LOG(LogGameMode, Error, TEXT("Failed to create new factory object"));

	return ModeFactory;
}

void ASessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}