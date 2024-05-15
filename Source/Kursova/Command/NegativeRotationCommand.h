// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "NegativeRotationCommand.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UNegativeRotationCommand : public UCommand
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* Actor) const override;
};
