// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>

/**
 * 
 */
class KURSOVA_API ExceptionWeaponFilter : public std::exception
{
public:
	// Exception constructor
	explicit ExceptionWeaponFilter(const std::string& Message) noexcept : Message(Message){}

	// Exception destructor override
	virtual ~ExceptionWeaponFilter() override = default;

	// Outputs exception message
	virtual const char* what() const noexcept override {
		return Message.c_str();
	}
	
private:
	// Stores exception message
	std::string Message;
};
