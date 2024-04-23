// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "Radio.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API ARadio : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
	ARadio() = default;

	UPROPERTY()
	ARadio* SelfInstance;

public:
	ARadio* InitializeRadio();
};
