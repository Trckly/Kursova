// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kursova/KursovaGameModeBase.h"
#include "Kursova/AI/AbstractFactory/GoblinCreator.h"
#include "SessionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API ASessionGameMode : public AKursovaGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGoblinCreator> GoblinCreatorClass;

	UPROPERTY()
	UGoblinCreator* GoblinCreator;
	
	TArray<IEnemyInterface*> Enemies;
	
public:
	
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
