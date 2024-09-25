// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Handler.h"
#include "UObject/NoExportTypes.h"
#include "AbstractHandler.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UAbstractHandler : public UObject, public IHandler
{
	GENERATED_BODY()
	
	IHandler* NextHandler;

public:
	
	virtual IHandler* SetNext(IHandler* Handler) override;
	virtual bool Handle(AAbstractItem* PickUpItem) override;
};
