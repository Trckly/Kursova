// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventListener.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "PositiveRotationCounter.generated.h"

UCLASS()
class KURSOVA_API APositiveRotationCounter : public AActor, public IEventListener
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APositiveRotationCounter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent* WidgetComponent;

	UPROPERTY(BlueprintReadWrite)
	float UpdateValue;

public:	
	virtual void Update(float Value) override;

};
