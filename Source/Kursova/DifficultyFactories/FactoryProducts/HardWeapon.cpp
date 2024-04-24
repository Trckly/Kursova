// Fill out your copyright notice in the Description page of Project Settings.


#include "HardWeapon.h"

// Sets default values
AHardWeapon::AHardWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = SkeletalMeshComponent;
}

// Called when the game starts or when spawned
void AHardWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHardWeapon::DetachFromActor(const FDetachmentTransformRules& DetachmentTransformRules)
{
	this->AActor::DetachFromActor(DetachmentTransformRules);
}

void AHardWeapon::Destroy()
{
	this->AActor::Destroy();
}

void AHardWeapon::SetActorEnableCollision(bool bNewActorEnableCollision)
{
	this->AActor::SetActorEnableCollision(bNewActorEnableCollision);
}

void AHardWeapon::AttachToComponent(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules,
	FName SocketName)
{
	this->AActor::AttachToComponent(Parent, AttachmentRules, SocketName);
}

