// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayer.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	AMainPlayerController();

	virtual void BeginPlay() override;

	void SetupInputComponent() override;

	//*****************************************
	void LookUp(float Value);

	void LookRight(float Value);

	void RotatePositive();
	void RotateNegative();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	AMainPlayer* ControlledPlayer;

	UPROPERTY(EditDefaultsOnly)
	float MouseSensivity;
	
};
