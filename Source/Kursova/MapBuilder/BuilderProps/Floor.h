// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Floor.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API AFloor : public AStaticMeshActor
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
