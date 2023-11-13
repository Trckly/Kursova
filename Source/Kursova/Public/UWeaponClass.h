// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExcWeaponCl.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "FWeaponUnit.h"
#include "Engine/DataTable.h"
#include "UWeaponClass.generated.h"

/**
 * 
 */

UCLASS()
class KURSOVA_API UWeaponClass : public UObject
{
	GENERATED_BODY()
private:
	TArray<FWeaponUnit*> Unit;

	UPROPERTY()
	TArray<FString> DataTableKeys = {"CZP10C", "Glock18", "AWM", "Beretta", "M16A2", "M16A4", "M700", "Makarov", "MP5",
		"MP7", "M870", "SIG", "TEC", "HK416", "Benelli"};
	
public:
	/// 
	///	Constructors/Destructors
	///
	UWeaponClass();
	UWeaponClass(FString Model, FString MainType, FString Subtype, int Capacity, FString Manufacturer, FString Caliber,
	             int Length, int Weight, int Price);

	//Copy constructor
	//Commented due to Unreal Engine GENERATED_BODY() macro policy
	//UWeaponClass(const UWeaponClass& Other);
	
	~UWeaponClass() = default;
	
	///
	///	Overloaded file read/write operators
	///
	friend std::ofstream& operator<<(std::ofstream& Fout, const UWeaponClass& Weapon);
	friend std::ifstream& operator>>(std::ifstream& Fin, UWeaponClass& Weapon);
};
