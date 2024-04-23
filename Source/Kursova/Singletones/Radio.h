// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "Radio.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API ARadio : public AActor
{
	GENERATED_BODY()
	
	ARadio();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;

	static ARadio* SelfInstance;

public:
	static ARadio* GetInstance(UWorld* World, TSubclassOf<ARadio> RadioClass);
};
