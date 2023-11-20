// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGameInstance.h"

UGenericGameInstance::UGenericGameInstance()
{
	const ConstructorHelpers::FObjectFinder<UDataTable> WeaponUnitDataObject(TEXT("DataTable'/Game/DataTables/"
																		 "WeaponConfig.WeaponConfig'"));
	if(WeaponUnitDataObject.Succeeded())
	{
		WeaponDataTable = WeaponUnitDataObject.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error reading data from WeaponConfig!"));
	}
	MapWeaponIndex = 0;
}

void UGenericGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Made it to Init"));
	Super::Init();
}
