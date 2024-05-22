// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Handler.generated.h"

class AAbstractItem;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
*/

class KURSOVA_API IHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual IHandler* SetNext(IHandler* Handler) = 0;
	virtual bool Handle(AAbstractItem* PickUpItem) = 0;
};
