// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>
/**
 * 
 */
class KURSOVA_API ExceptionWeaponOutput : public std::exception
{
public:
	ExceptionWeaponOutput(std::string Message) : Message(Message){}
	~ExceptionWeaponOutput() = default;

	const char* what() const noexcept override {
		return Message.c_str();
	}
private:
	std::string Message;
};
