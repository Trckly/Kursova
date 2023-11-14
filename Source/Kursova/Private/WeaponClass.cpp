// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponClass.h"

WeaponClass::WeaponClass(std::string Model, std::string MainType, std::string Subtype, unsigned Capacity,
	std::string Manufacturer, unsigned Caliber, unsigned int Dimension[3], unsigned int Weight, unsigned int Price)
		: Model(Model), MainType(MainType), Subtype(Subtype), Capacity(Capacity), Manufacturer(Manufacturer),
		Caliber(Caliber), Dimension{Dimension[0], Dimension[1], Dimension[2]}, Weight(Weight), Price(Price)
{}

WeaponClass::WeaponClass(const WeaponClass& Other)
{
	Model = Other.Model;
	MainType = Other.MainType;
	Subtype = Other.Subtype;
	Capacity = Other.Capacity;
	Manufacturer = Other.Manufacturer;
	Caliber = Other.Caliber;
	for (int i = 0; i < 3; i++) {
		Dimension[i] = Other.Dimension[i];
	}
	Weight = Other.Weight;
	Price = Other.Price;
}

std::ofstream& operator<<(std::ofstream& Fout, const WeaponClass& Weapon)
{
	Fout << Weapon.Model << ' ' << Weapon.MainType << ' ' << Weapon.Subtype << ' ' << Weapon.Capacity << ' '
	<< Weapon.Manufacturer << ' ' << Weapon.Caliber << ' ' << Weapon.Dimension[0] << ' ' << Weapon.Dimension[1] << ' '
	<< Weapon.Dimension[2] << ' ' << Weapon.Weight << ' ' << Weapon.Price;
	return Fout;
}

std::ifstream& operator>>(std::ifstream& Fin, WeaponClass& Weapon) {
	std::string line;
    
	// Read a line from the file
	if (std::getline(Fin, line)) {
		std::istringstream iss(line);
        
		if (!(iss >> Weapon.Model >> Weapon.MainType >> Weapon.Subtype
				>> Weapon.Capacity >> Weapon.Manufacturer >> Weapon.Caliber
				>> Weapon.Dimension[0] >> Weapon.Dimension[1] >> Weapon.Dimension[2]
				>> Weapon.Weight >> Weapon.Price))
		{
			throw ExcWeaponCl("Corrupted file! Missing data!");
		}
	} else {
		throw ExcWeaponCl("Failed to read a line from the file!");
	}

	return Fin;
}

