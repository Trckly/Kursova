// Fill out your copyright notice in the Description page of Project Settings.


#include "PositiveRotationCounter.h"

// Sets default values
APositiveRotationCounter::APositiveRotationCounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	RootComponent = WidgetComponent;
}

// Called when the game starts or when spawned
void APositiveRotationCounter::BeginPlay()
{
	Super::BeginPlay();
}

void APositiveRotationCounter::Update(float Value)
{
	UpdateValue += Value;
}

