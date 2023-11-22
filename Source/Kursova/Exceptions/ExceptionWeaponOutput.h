// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>

/**
 Exception class for Input/Output exceptions of AWeaponClass class
 */
class KURSOVA_API ExceptionWeaponOutput : public std::exception
{
public:
	// Exception constructor
	explicit ExceptionWeaponOutput(const std::string& Message) noexcept : Message(Message){}

	// Exception destructor override
	virtual ~ExceptionWeaponOutput() override = default;

	// Outputs exception message
	virtual const char* what() const noexcept override {
		return Message.c_str();
	}
	
private:
	// Stores exception message
	std::string Message;
};
