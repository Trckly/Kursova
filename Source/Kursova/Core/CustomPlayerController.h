// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class KURSOVA_API ACustomPlayerController : public AMainPlayerController
{
	GENERATED_BODY()

	UFUNCTION(Reliable, Client)
	virtual void BeginPlay() override;
  
};