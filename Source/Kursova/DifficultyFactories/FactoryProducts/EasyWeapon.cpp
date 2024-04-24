// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyWeapon.h"

// Sets default values
AEasyWeapon::AEasyWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void AEasyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEasyWeapon::DetachFromActor(const FDetachmentTransformRules& DetachmentTransformRules)
{
	this->AActor::DetachFromActor(DetachmentTransformRules);
}

void AEasyWeapon::Destroy()
{
	this->AActor::Destroy();
}

void AEasyWeapon::SetActorEnableCollision(bool bNewActorEnableCollision)
{
	this->AActor::SetActorEnableCollision(bNewActorEnableCollision);
}

void AEasyWeapon::AttachToComponent(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules,
	FName SocketName)
{
	this->AActor::AttachToComponent(Parent, AttachmentRules, SocketName);
}

