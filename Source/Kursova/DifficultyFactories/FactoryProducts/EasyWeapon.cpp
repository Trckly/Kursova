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

