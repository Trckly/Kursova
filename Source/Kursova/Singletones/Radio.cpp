// Fill out your copyright notice in the Description page of Project Settings.

#include "Radio.h"

#include "Kismet/KismetMathLibrary.h"

ARadio* ARadio::SelfInstance = nullptr;

ARadio::ARadio()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = SkeletalMeshComponent;
}

ARadio* ARadio::GetInstance(UWorld* World, TSubclassOf<ARadio> RadioClass)
{
	if(!SelfInstance)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		SelfInstance =  World->SpawnActor<ARadio>(RadioClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	}

	return SelfInstance;
}
