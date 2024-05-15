// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "PositiveRotationCommand.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UPositiveRotationCommand : public UCommand
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* Actor) const override;
};
