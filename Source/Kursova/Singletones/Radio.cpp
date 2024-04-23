// Fill out your copyright notice in the Description page of Project Settings.

#include "Radio.h"

ARadio* ARadio::SelfInstance = nullptr;

ARadio::ARadio()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Capsule Collider"));
	RootComponent = BoxComponent;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

ARadio* ARadio::GetInstance(UWorld* World, TSubclassOf<ARadio> RadioClass)
{
	if(SelfInstance == nullptr)
	{
		SelfInstance = World->SpawnActor<ARadio>(RadioClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}
	
	return SelfInstance;
}
