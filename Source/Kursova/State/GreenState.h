// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "UObject/NoExportTypes.h"
#include "GreenState.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UGreenState : public UObject, public IState
{
	GENERATED_BODY()
	
public:
	virtual void ChangeColor(AActor* Actor) override;
};
