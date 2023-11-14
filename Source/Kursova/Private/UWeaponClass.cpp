// Fill out your copyright notice in the Description page of Project Settings.


#include "UWeaponClass.h"

UWeaponClass::UWeaponClass()
{
	UDataTable* WeaponData;
	ConstructorHelpers::FObjectFinder<UDataTable> WeaponUnitDataObject(TEXT("DataTable'/Game/DataTables/"
																		 "WeaponConfig.WeaponConfig'"));
	if(WeaponUnitDataObject.Succeeded())
	{
		WeaponData = WeaponUnitDataObject.Object;

		const FString ContextString(TEXT("Weapon Data Context")); 
		for(int i = 0; i < DataTableKeys.Num(); ++i)
		{
			Unit.Push(WeaponData->FindRow<FWeaponUnit>(FName(*DataTableKeys[i]), ContextString,
				true));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error reading data from WeaponConfig!"));
	}
}

UWeaponClass::UWeaponClass(FString Model, FString MainType, FString Subtype, int Capacity,FString Manufacturer,
	FString Caliber, int Length, int Weight,int Price)
{
	FWeaponUnit* Temp = new FWeaponUnit;
	Temp->Model = Model;
	Temp->MainType = MainType;
	Temp->Subtype = Subtype;
	Temp->Capacity = Capacity;
	Temp->Manufacturer = Manufacturer;
	Temp->Caliber = Caliber;
	Temp->Length = Length;
	Temp->Weight = Weight;
	Temp->Price = Price;

	Unit.Push(Temp);
}

// UWeaponClass::UWeaponClass(const UWeaponClass& Other)
// {
// 	if(Unit.Num() > 0)
// 		Unit.Empty();
//
// 	for (int i = 0; i < Other.Unit.Num(); ++i)
// 	{
// 		Unit.Push(Other.Unit[i]);
// 	}
// }

std::ofstream& operator<<(std::ofstream& Fout, const UWeaponClass& Weapon)
{
	FString Result;
	for(int i = 0; i < Weapon.Unit.Num(); ++i)
	{
		Result += Weapon.Unit[i]->Model + ' ' + Weapon.Unit[i]->MainType + ' ' + Weapon.Unit[i]->Subtype + ' ' +
			FString::FromInt(Weapon.Unit[i]->Capacity) + ' '+ Weapon.Unit[i]->Manufacturer + ' ' +
				Weapon.Unit[i]->Caliber + ' ' + FString::FromInt(Weapon.Unit[i]->Length) + ' ' +
					FString::FromInt(Weapon.Unit[i]->Weight) + ' ' + FString::FromInt(Weapon.Unit[i]->Price) + '\n';
	}
	Fout << *Result;
	return Fout;
}

std::ifstream& operator>>(std::ifstream& Fin, UWeaponClass& Weapon) {
	std::string line;
    
	// Read a line from the file

	while (std::getline(Fin, line))
	{
		std::istringstream iss(line);
		TArray<std::string> Temp;
		Temp.SetNum(9);
		if (!(iss >> Temp[0] >> Temp[1] >> Temp[2] >> Temp[3] >> Temp[4] >> Temp[5]	>> Temp[6] >> Temp[7] >> Temp[8]))
		{
			throw ExcWeaponCl("Corrupted file! Missing data!");
		}
		
		FWeaponUnit* TempUnit = new FWeaponUnit;
		TempUnit->Model = Temp[0].c_str();
		TempUnit->MainType = Temp[1].c_str();
		TempUnit->Subtype = Temp[2].c_str();
		TempUnit->Capacity = std::atoi(Temp[3].c_str());
		TempUnit->Manufacturer = Temp[4].c_str();
		TempUnit->Caliber = Temp[5].c_str();
		TempUnit->Length = std::atoi(Temp[6].c_str());
		TempUnit->Weight = std::atoi(Temp[7].c_str());
		TempUnit->Price = std::atoi(Temp[8].c_str());
		Weapon.Unit.Push(TempUnit);
	}

	return Fin;
}

