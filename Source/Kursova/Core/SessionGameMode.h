// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kursova/KursovaGameModeBase.h"
#include "Kursova/AI/AbstractFactory/GoblinCreator.h"
#include "Kursova/AI/AbstractFactory/SkeletonCreator.h"
#include "Kursova/DifficultyFactories/EasyModeFactory.h"
#include "Kursova/DifficultyFactories/ModeFactory.h"
#include "Kursova/Items/CoolItem.h"
#include "Kursova/MapBuilder/MainMapBuilder.h"
#include "SessionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API ASessionGameMode : public AKursovaGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TSubclassOf<UGoblinCreator> GoblinCreatorClass;

	UPROPERTY()
	UGoblinCreator* GoblinCreator = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TSubclassOf<USkeletonCreator> SkeletonCreatorClass;
	
	UPROPERTY()
	USkeletonCreator* SkeletonCreator = nullptr;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TSubclassOf<UGoblinCreator> GoblinCreatorClass;
	//
	// UPROPERTY()
	// UGoblinCreator* GoblinCreator;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items")
	TSubclassOf<ACoolItem> CoolItemClass;
	
	UPROPERTY()
	ACoolItem* CoolItem = nullptr;
	
	TArray<IEnemyInterface*> Enemies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	int NumberOfBiters = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	int NumberOfShooters = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	int NumberOfSuicidal = 5;

	UPROPERTY()
	FString DifficultyMode;

	UPROPERTY()
	TScriptInterface<IModeFactory> ModeFactory;
	
	virtual void BeginPlay() override;
	
	void CreateEnemies(IIEnemyCreator* EnemyCreator);

	UPROPERTY()
	FTimerHandle ScoreTimerHandle;
	
	UFUNCTION()
	void CreateHardPoint();

	UFUNCTION()
	void HandleHardPointCapture();

	UFUNCTION()
	TScriptInterface<IModeFactory> SetDifficultyMode(const FString& Difficulty);
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UMainMapBuilder> MainMapBuilderClass;

	UPROPERTY()
	UMainMapBuilder* MainMapBuilder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector2D GeneralUniversalDimensions = FVector2D(10.f, 10.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Difficulty")
	TSubclassOf<UEasyModeFactory> EasyModeFactoryClass;

	
};
