// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeaponClass.h"
#include "Engine/GameInstance.h"
#include "GenericGameInstance.generated.h"

class AMainPlayer;
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
	
public:
	UPROPERTY()
	UDataTable* WeaponDataTable;
};
