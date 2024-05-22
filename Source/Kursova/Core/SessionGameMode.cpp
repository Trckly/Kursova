// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameMode.h"

#include "MainPlayer.h"
#include "Kursova/AI/Strategy/EnemyCreationMode.h"
#include "Kursova/AI/Strategy/FiniteEnemyCreation.h"
#include "Kursova/AI/Strategy/InfiniteEnemyCreation.h"
#include "Kursova/DifficultyFactories/EasyModeFactory.h"
#include "Kursova/DifficultyFactories/HardModeFactory.h"
#include "Kursova/DifficultyFactories/MediumModeFactory.h"

void ASessionGameMode::CreateEnemies(IIEnemyCreator* EnemyCreator, IEnemyCreationMode* CreationMode)
{
	CreationMode->CreateEnemies(EnemyCreator, &Enemies);
	
	// while(NumberOfBiters--)
	// {
	// 	IEnemyInterface* NewEnemy = EnemyCreator->CreateEnemies(EEnemyType::Biting);
	// 	if(NewEnemy)
	// 		Enemies.Add(NewEnemy);
	// 	else
	// 		NumberOfBiters++;
	// }
	// while(NumberOfSuicidal--)
	// {
	// 	IEnemyInterface* NewEnemy = EnemyCreator->CreateEnemies(EEnemyType::Exploding);
	// 	if(NewEnemy)
	// 		Enemies.Add(NewEnemy);
	// 	else
	// 		NumberOfSuicidal++;		
	// }
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
			//MainMapBuilder->CreateForest(GeneralUniversalDimensions);
			MainMapBuilder->BuildGeneralWalls(GeneralUniversalDimensions);
			MainMapBuilder->BuildObstacles(GeneralUniversalDimensions);
			MainMapBuilder->PlaceRadio(GeneralUniversalDimensions);
		}
	}

	AMainPlayer* MainPlayer = Cast<AMainPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
	if(MainPlayer)
		MainPlayer->SetDifficultyModeDelegate.BindDynamic(this, &ASessionGameMode::SetDifficultyMode);
	else
		UE_LOG(LogGameMode, Error, TEXT("Failed to get MainPlayer reference"));
	
	if(CoolItemClass)
	{
		CreateHardPoint();
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	if(HealthItemClass)
	{
		for(int i = 0; i < 5; i++)
		{
			HealthItems.Add(AHealthItem::CreateInstance(GetWorld(), HealthItemClass, GeneralUniversalDimensions * 400.f, SpawnParameters));
		}
	}
	if(ArmorItemClass)
	{
		for(int i = 0; i < 5; i++)
		{
			ArmorItems.Add(AArmorItem::CreateInstance(GetWorld(), ArmorItemClass, GeneralUniversalDimensions * 400.f, SpawnParameters));
		}
	}
}

TScriptInterface<IModeFactory> ASessionGameMode::SetDifficultyMode(const FString& Difficulty)
{
	if(DifficultyMode == "")
		DifficultyMode = Difficulty;

	IEnemyCreationMode* EnemyCreationMode = nullptr;
	if(DifficultyMode == "Easy")
	{
		ModeFactory = NewObject<UEasyModeFactory>(this, EasyModeFactoryClass, TEXT("Easy Mode Factory"));
		EnemyCreationMode = NewObject<UFiniteEnemyCreation>(this, UFiniteEnemyCreation::StaticClass(), TEXT("Finite Creation"));
	}
	if(DifficultyMode == "Medium")
	{
		ModeFactory = NewObject<UMediumModeFactory>(this, MediumModeFactoryClass, TEXT("Medium Mode Factory"));
		EnemyCreationMode = NewObject<UFiniteEnemyCreation>(this, UFiniteEnemyCreation::StaticClass(), TEXT("Finite Creation"));
	}
	if(DifficultyMode == "Hard")
	{
		ModeFactory = NewObject<UHardModeFactory>(this, HardModeFactoryClass, TEXT("Hard Mode Factory"));
		EnemyCreationMode = NewObject<UInfiniteEnemyCreation>(this, UInfiniteEnemyCreation::StaticClass(), TEXT("Finite Creation"));
	}
	if(EnemyCreationMode)
		EnemyCreationMode->OnEnemyUpdated.BindDynamic(this, &ASessionGameMode::UpdateEnemies);

	if(!ModeFactory)
		UE_LOG(LogGameMode, Error, TEXT("Failed to create new factory object"));
	
	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	
	if(MapName == FString("GoblinsMap"))
	{
		if(GoblinCreatorClass)
		{
			GoblinCreator = UGoblinCreator::Create(this, GoblinCreatorClass, GeneralUniversalDimensions * 400.f);

			if(GoblinCreator && EnemyCreationMode)
				CreateEnemies(GoblinCreator, EnemyCreationMode);
		}
	}else if(MapName == FString("SkeletonsMap"))
	{
		if(SkeletonCreatorClass)
		{
			SkeletonCreator = USkeletonCreator::Create(this, SkeletonCreatorClass, GeneralUniversalDimensions * 400.f);

			if(SkeletonCreator && EnemyCreationMode)
				CreateEnemies(SkeletonCreator, EnemyCreationMode);
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to create enemies!"));


	return ModeFactory;
}

void ASessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ASessionGameMode::UpdateEnemies()
{
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(MainPlayer)
	{
		if(Enemies.Num() != 0)
		{
			MainPlayer->Attach(Cast<IEnemyObserver>(Enemies.Last()));
		}
	}
}
