// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KursovaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API AKursovaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AKursovaGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float Hour = 0.f;
};
