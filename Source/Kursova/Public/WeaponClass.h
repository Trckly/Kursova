// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExcWeaponCl.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * 
 */
class KURSOVA_API WeaponClass
{
private:
	std::string Model;
	std::string MainType;
	std::string Subtype;
	unsigned int Capacity;
	std::string Manufacturer;
	unsigned int Caliber;
	unsigned int Dimension[3];
	unsigned int Weight;
	unsigned int Price;
	
public:
	/// 
	///	Constructors/Destructors
	///
	WeaponClass() = default;

	WeaponClass(std::string Model, std::string MainType, std::string Subtype, unsigned int Capacity,
		std::string Manufacturer, unsigned int Caliber, unsigned int Dimension[3], unsigned int Weight,
		unsigned int Price);

	//Copy constructor
	WeaponClass(const WeaponClass& Other);
	
	~WeaponClass() = default;

	///
	///	Overloaded file read/write operators
	///
	friend std::ofstream& operator<<(std::ofstream& Fout, const WeaponClass& Weapon);
	friend std::ifstream& operator>>(std::ifstream& Fin, WeaponClass& Weapon);
};
