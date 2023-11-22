// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>
/**
 * 
 */
class KURSOVA_API ExceptionPlayerSort : public std::exception
{
public:
	ExceptionPlayerSort(std::string ErrorMessage);
	virtual ~ExceptionPlayerSort() override = default;

	virtual const char* what() const noexcept override {
		return Message.c_str();
	}
private:
	std::string Message;
};
