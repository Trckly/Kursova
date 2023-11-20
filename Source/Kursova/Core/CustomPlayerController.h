// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class KURSOVA_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

	UFUNCTION(Reliable, Client)
	virtual void BeginPlay() override;
	
};
