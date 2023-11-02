// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>
/**
 * 
 */
class KURSOVA_API ExcWeaponCl : public std::exception
{
public:
	ExcWeaponCl(std::string Message) : Message(Message){}
	~ExcWeaponCl() = default;

	const char* what() const noexcept override {
		return Message.c_str();
	}
private:
	std::string Message;
};
