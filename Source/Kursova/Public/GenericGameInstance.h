// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeaponClass.h"
#include "Engine/GameInstance.h"
#include "GenericGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UGenericGameInstance : public UGameInstance
{
	GENERATED_BODY()

	int MapWeaponIndex;
protected:
	UGenericGameInstance();
	
	// UFUNCTION(BlueprintCallable)
	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite)
	TMap<int, AWeaponClass*> WeaponHolder;

	UPROPERTY()
	TArray<FString> DataTableKeys = {"CZP10C", "Glock18", "AWM", "Beretta", "M16A2", "M16A4", "M700", "Makarov", "MP5",
		"MP7", "M870", "SIG", "TEC", "HK416", "Benelli"};

	UPROPERTY()
	UDataTable* WeaponDataTable;
};
