// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>

/**
  Exception class for edit properties of weapon exceptions
 */
class KURSOVA_API ExceptionPropertiesEdit : public std::exception
{
public:
public:
	// Exception constructor
	explicit ExceptionPropertiesEdit(const std::string& Message) noexcept : Message(Message){}

	// Exception destructor override
	virtual ~ExceptionPropertiesEdit() override = default;

	// Outputs exception message
	virtual const char* what() const noexcept override {
		return Message.c_str();
	}
	
private:
	// Stores exception message
	std::string Message;
};
