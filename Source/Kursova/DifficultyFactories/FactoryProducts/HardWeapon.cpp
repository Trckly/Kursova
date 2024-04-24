// Fill out your copyright notice in the Description page of Project Settings.


#include "HardWeapon.h"

// Sets default values
AHardWeapon::AHardWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHardWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHardWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

