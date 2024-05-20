// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "RedState.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API URedState : public UObject, public IState
{
	GENERATED_BODY()

public:
	
	virtual void ChangeColor(AActor* Actor) override;
};
