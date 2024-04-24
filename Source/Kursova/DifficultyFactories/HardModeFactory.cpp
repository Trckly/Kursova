// Fill out your copyright notice in the Description page of Project Settings.


#include "HardModeFactory.h"

IWeaponInterface* UHardModeFactory::CreateWeapon()
{
	AHardWeapon* EasyWeapon = GetWorld()->SpawnActor<AHardWeapon>(HardWeaponClass, FVector::ZeroVector,
		FRotator::ZeroRotator);
	if (!EasyWeapon) UE_LOG(LogTemp, Error, TEXT("Failed to spawn HardWeapon instance"));
	return EasyWeapon;
}
