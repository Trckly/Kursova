// Fill out your copyright notice in the Description page of Project Settings.


#include "MediumModeFactory.h"

IWeaponInterface* UMediumModeFactory::CreateWeapon()
{
	AMediumWeapon* EasyWeapon = GetWorld()->SpawnActor<AMediumWeapon>(MediumWeaponClass, FVector::ZeroVector,
		FRotator::ZeroRotator);
	if (!EasyWeapon) UE_LOG(LogTemp, Error, TEXT("Failed to spawn MediumWeapon instance"));
	return EasyWeapon;
}
