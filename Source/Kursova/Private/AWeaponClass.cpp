// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeaponClass.h"

AWeaponClass::AWeaponClass(FString Model, FString MainType, FString Subtype, int Capacity,FString Manufacturer,
                           float Caliber, int Length, int Weight, int Price)
{
	WeaponUnit.Model = Model;
	WeaponUnit.MainType = MainType;
	WeaponUnit.Subtype = Subtype;
	WeaponUnit.Capacity = Capacity;
	WeaponUnit.Manufacturer = Manufacturer;
	WeaponUnit.Caliber = Caliber;
	WeaponUnit.Length = Length;
	WeaponUnit.Weight = Weight;
	WeaponUnit.Price = Price;
}

void AWeaponClass::InitWithStruct(const FWeaponUnit& StructItem) noexcept
{
	WeaponUnit.Model = StructItem.Model;
	WeaponUnit.MainType = StructItem.MainType;
	WeaponUnit.Subtype = StructItem.Subtype;
	WeaponUnit.Capacity = StructItem.Capacity;
	WeaponUnit.Manufacturer = StructItem.Manufacturer;
	WeaponUnit.Caliber = StructItem.Caliber;
	WeaponUnit.Length = StructItem.Length;
	WeaponUnit.Weight = StructItem.Weight;
	WeaponUnit.Price = StructItem.Price;
}

const FWeaponUnit& AWeaponClass::GetStructure() noexcept
{
	return WeaponUnit;
}

void AWeaponClass::EditStructure(const TArray<FText>& NewProperties) noexcept
{
	WeaponUnit.Model = NewProperties[0].ToString();
	WeaponUnit.MainType = NewProperties[1].ToString();
	WeaponUnit.Subtype = NewProperties[2].ToString();
	WeaponUnit.Capacity = FCString::Atoi(*NewProperties[3].ToString());
	WeaponUnit.Manufacturer = NewProperties[4].ToString();
	WeaponUnit.Caliber = FCString::Atof(*NewProperties[5].ToString());
	WeaponUnit.Length = FCString::Atoi(*NewProperties[6].ToString());
	WeaponUnit.Weight = FCString::Atoi(*NewProperties[7].ToString());
	WeaponUnit.Price = FCString::Atoi(*NewProperties[8].ToString());
}

// AWeaponClass::AWeaponClass(const AWeaponClass& Other)
// {
// 	WeaponUnit = Other.WeaponUnit;
// }

