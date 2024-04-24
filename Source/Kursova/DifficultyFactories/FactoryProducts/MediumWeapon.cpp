// Fill out your copyright notice in the Description page of Project Settings.


#include "MediumWeapon.h"

// Sets default values
AMediumWeapon::AMediumWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void AMediumWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMediumWeapon::DetachFromActor(const FDetachmentTransformRules& DetachmentTransformRules)
{
	this->AActor::DetachFromActor(DetachmentTransformRules);
}

void AMediumWeapon::Destroy()
{
	this->AActor::Destroy();
}

void AMediumWeapon::SetActorEnableCollision(bool bNewActorEnableCollision)
{
	this->AActor::SetActorEnableCollision(bNewActorEnableCollision);
}

void AMediumWeapon::AttachToComponent(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules,
	FName SocketName)
{
	this->AActor::AttachToComponent(Parent, AttachmentRules, SocketName);
}
