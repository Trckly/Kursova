// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../KursovaGameModeBase.h"
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
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UMainMapBuilder> MainMapBuilderClass;

	UMainMapBuilder* MainMapBuilder;
};
