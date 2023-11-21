// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExceptionWeaponOutput.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "FWeaponUnit.h"
#include "Engine/DataTable.h"
#include "AWeaponClass.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class KURSOVA_API AWeaponClass : public AActor, public FWeaponUnit
{
	GENERATED_BODY()
		
protected:
	UPROPERTY(EditDefaultsOnly)
	FWeaponUnit WeaponUnit;
	
public:
	/// 
	///	Constructors/Destructors
	///
	AWeaponClass() = default;
	AWeaponClass(FString Model, FString MainType, FString Subtype, int Capacity, FString Manufacturer, float Caliber,
	             int Length, int Weight, int Price);
	
	//Copy constructor
	//Commented due to Unreal Engine GENERATED_BODY() macro policy
	//UWeaponClass(const UWeaponClass& Other);
	
	~AWeaponClass() = default;

	void InitWithStruct(const FWeaponUnit& StructItem);

	UFUNCTION(BlueprintCallable)
	const FWeaponUnit& GetStructure();

	void EditStructure(const TArray<FText>& NewProperties);
	
	///
	///	Overloaded file read/write operators
	///
	friend std::ofstream& operator<<(std::ofstream& Fout, const AWeaponClass& Weapon);
	friend std::ifstream& operator>>(std::ifstream& Fin, AWeaponClass& Weapon);
};
