// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

ACube::ACube()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMeshComponent;
}

void ACube::BeginPlay()
{
	Super::BeginPlay();
	
	ChangeColor();
}

void ACube::Tick(float DeltaSeconds)
{
	if(bPositiveRotation)
	{
		SetActorRotation(GetActorRotation() + FRotator(0.f, RotationSpeed, 0.f) * DeltaSeconds);
		GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, TEXT("Positive Rotation"));
	}
	if(bNegativeRotation)
	{
		SetActorRotation(GetActorRotation() + FRotator(0.f, -RotationSpeed, 0.f) * DeltaSeconds);
	}
}

ICubeInterface* ACube::CreateCube(TSubclassOf<UObject> CubeClass, UWorld* World)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	return World->SpawnActor<ACube>(CubeClass, FVector(391.f, -29.f, 173.f), FRotator::ZeroRotator, SpawnParameters);
}

void ACube::DestroyCube()
{
	Destroy();
}

UStaticMeshComponent* ACube::GetStaticMeshComponent()
{
	return StaticMeshComponent;
}

void ACube::ChangeColor()
{
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMeshComponent->GetMaterial(0), nullptr);

	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);

	StaticMeshComponent->SetMaterial(0, DynamicMaterial);
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

