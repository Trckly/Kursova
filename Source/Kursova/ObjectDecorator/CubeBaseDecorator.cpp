// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeBaseDecorator.h"

// Sets default values
ACubeBaseDecorator::ACubeBaseDecorator()
{
}

void ACubeBaseDecorator::BeginPlay()
{
}

void ACubeBaseDecorator::Wrap(ICubeInterface* Source)
{
	Wrappee = Source;
}

ICubeInterface* ACubeBaseDecorator::CreateCube(ICubeInterface* Source, TSubclassOf<UObject> CubeClass, UWorld* World)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	auto NewCube =  World->SpawnActor<ACubeBaseDecorator>(CubeClass, FVector(391.f, -29.f, 173.f), FRotator::ZeroRotator, SpawnParameters);

	Wrap(NewCube);
	
	ChangeColor();
	
	return NewCube;
}

void ACubeBaseDecorator::DestroyCube()
{
	Destroy();
}

void ACubeBaseDecorator::ChangeColor()
{
	Wrappee->ChangeColor();
}

UStaticMeshComponent* ACubeBaseDecorator::GetStaticMeshComponent()
{
	return Wrappee->GetStaticMeshComponent();
}

