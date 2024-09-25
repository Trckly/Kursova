// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Kursova/State/State.h"
#include "Radio.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API ARadio : public AActor
{
	GENERATED_BODY()
	
	ARadio();
	
	static ARadio* SelfInstance;

	IState* State;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;

public:
	static ARadio* GetInstance(UWorld* World, TSubclassOf<ARadio> RadioClass);

	void ChangeState(IState* OtherState);
};
