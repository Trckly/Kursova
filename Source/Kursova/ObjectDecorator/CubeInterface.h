// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CubeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCubeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KURSOVA_API ICubeInterface
{
	GENERATED_BODY()

public:
	virtual void ChangeColor() = 0;
	virtual UStaticMeshComponent* GetStaticMeshComponent() = 0;
};
