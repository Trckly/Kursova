// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "FWeaponUnit.h"
#include "Engine/DataTable.h"
#include "AWeaponClass.generated.h"

/**
 Universal class for all instances of weapon on the scene
 */
UCLASS(BlueprintType)
class KURSOVA_API AWeaponClass : public AActor
{
	GENERATED_BODY()
		
protected:
	// Instance of weapon properties structure
	UPROPERTY(EditDefaultsOnly)
	FWeaponUnit WeaponUnit;
	
public:
	/// 
	///	Constructors/Destructors
	///
	
	//Default constructor
	AWeaponClass() noexcept = default;

	//Constructor with instant initialization
	AWeaponClass(FString Model, FString MainType, FString Subtype, int Capacity, FString Manufacturer, float Caliber,
	             int Length, int Weight, int Price);
	
	// Copy constructor
	// Commented due to Unreal Engine GENERATED_BODY() macro policy
	// AWeaponClass(const AWeaponClass& Other);

	//Default destructor
	virtual ~AWeaponClass() noexcept override = default;

	///
	/// General functions for working with class
	///

	// Initializes unit with prepared properties
	// structure
	void InitWithStruct(const FWeaponUnit& StructItem) noexcept;

	// Returns reference to stored properties
	// structure (read-only)
	UFUNCTION(BlueprintCallable)
	const FWeaponUnit& GetStructure() noexcept;

	// Edits properties structure with an array of strings
	void EditStructure(const TArray<FText>& NewProperties) noexcept;

	// Populates weapon properties from keyboard
	void WriteFromKeyboard() noexcept;

	
	///
	///	Overloaded file read/write operators
	///

	// Overloaded write
	friend std::ofstream& operator<<(std::ofstream& Fout, const AWeaponClass& Weapon);

	// Overloaded read
	friend std::ifstream& operator>>(std::ifstream& Fin, AWeaponClass& Weapon);
};
