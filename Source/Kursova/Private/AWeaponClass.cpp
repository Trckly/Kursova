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

std::ofstream& operator<<(std::ofstream& Fout, const AWeaponClass& Weapon)
{
	FString Result;
	Result += Weapon.WeaponUnit.Model + ' ' + Weapon.WeaponUnit.MainType + ' ' + Weapon.WeaponUnit.Subtype + ' ' +
		FString::FromInt(Weapon.WeaponUnit.Capacity) + ' '+ Weapon.WeaponUnit.Manufacturer + ' ' +
			FString(std::to_string(Weapon.WeaponUnit.Caliber).c_str()) + ' ' +
				FString::FromInt(Weapon.WeaponUnit.Length) + ' ' + FString::FromInt(Weapon.WeaponUnit.Weight) + ' '
					+ FString::FromInt(Weapon.WeaponUnit.Price) + '\n';
	Fout << *Result;
	return Fout;
}

std::ifstream& operator>>(std::ifstream& Fin, AWeaponClass& Weapon) {
	std::string line;
    
	// Read a line from the file

	if (std::getline(Fin, line))
	{
		std::istringstream iss(line);
		TArray<std::string> Temp;
		Temp.SetNum(9);
		if (!(iss >> Temp[0] >> Temp[1] >> Temp[2] >> Temp[3] >> Temp[4] >> Temp[5]	>> Temp[6] >> Temp[7] >> Temp[8]))
		{
			throw ExceptionWeaponOutput("Corrupted file! Missing data!");
		}
		
		Weapon.WeaponUnit.Model = Temp[0].c_str();
		Weapon.WeaponUnit.MainType = Temp[1].c_str();
		Weapon.WeaponUnit.Subtype = Temp[2].c_str();
		Weapon.WeaponUnit.Capacity = std::atoi(Temp[3].c_str());
		Weapon.WeaponUnit.Manufacturer = Temp[4].c_str();
		Weapon.WeaponUnit.Caliber = std::atof(Temp[5].c_str());
		Weapon.WeaponUnit.Length = std::atoi(Temp[6].c_str());
		Weapon.WeaponUnit.Weight = std::atoi(Temp[7].c_str());
		Weapon.WeaponUnit.Price = std::atoi(Temp[8].c_str());
	}
	else
	{
		throw ExceptionWeaponOutput("Cannot read data from file!");
	}

	return Fin;
}

