// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>
/**
 * 
 */
class KURSOVA_API ExceptionPlayerWidget : public std::exception
{
public:
	ExceptionPlayerWidget(std::string ErrorMessage);
	virtual ~ExceptionPlayerWidget() override = default;

	virtual const char* what() const noexcept override {
		return Message.c_str();
	}
private:
	std::string Message;
};
