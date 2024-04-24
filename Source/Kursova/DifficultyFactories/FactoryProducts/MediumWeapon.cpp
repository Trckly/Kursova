// Fill out your copyright notice in the Description page of Project Settings.


#include "MediumWeapon.h"

// Sets default values
AMediumWeapon::AMediumWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMediumWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMediumWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

