// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

#include "Kismet/GameplayStatics.h"
#include "Kursova/Observer/PositiveRotationCounter.h"

ACube::ACube()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMeshComponent;
}

void ACube::BeginPlay()
{
	Super::BeginPlay();
	
	SubscriptionManager = NewObject<USubscriptionManager>();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APositiveRotationCounter::StaticClass(), OutActors);

	for (const auto OutActor : OutActors)
	{
		IEventListener* EventListener = Cast<IEventListener>(OutActor);
		SubscriptionManager->Subscribe(EventListener);
	}
}

void ACube::Tick(float DeltaSeconds)
{
	if(bPositiveRotation)
	{
		SetActorRotation(GetActorRotation() + FRotator(0.f, RotationSpeed, 0.f) * DeltaSeconds);
		SubscriptionManager->Notify(RotationSpeed * DeltaSeconds);
	}
	if(bNegativeRotation)
	{
		SetActorRotation(GetActorRotation() + FRotator(0.f, -RotationSpeed, 0.f) * DeltaSeconds);
		SubscriptionManager->Notify(-RotationSpeed * DeltaSeconds);
	}
}

UStaticMeshComponent* ACube::GetStaticMeshComponent()
{
	return StaticMeshComponent;
}

void ACube::ChangeState(IState* otherState)
{
	State = otherState;
	State->ChangeColor(this);
}

void ACube::AddPositiveRotation()
{
	bPositiveRotation = true;
	bNegativeRotation = false;
}

void ACube::AddNegativeRotation()
{
	bNegativeRotation = true;
	bPositiveRotation = false;
}

UMaterialInstanceDynamic* ACube::GetDynamicMaterial()
{
	return DynamicMaterial;
}

void ACube::SetDynamicMaterial(UMaterialInstanceDynamic* Material)
{
	if(!DynamicMaterial)
		DynamicMaterial = Material;
}

