// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyModeFactory.h"


IWeaponInterface* UEasyModeFactory::CreateWeapon()
{
	AEasyWeapon* EasyWeapon = GetWorld()->SpawnActor<AEasyWeapon>(EasyWeaponClass, FVector::ZeroVector,
		FRotator::ZeroRotator);
	if (!EasyWeapon) UE_LOG(LogTemp, Error, TEXT("Failed to spawn EasyWeapon instance"));
	return EasyWeapon;
}
