// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeBaseDecorator.h"
#include "BlueCubeDecorator.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API ABlueCubeDecorator : public ACubeBaseDecorator
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	virtual ICubeInterface* CreateCube(ICubeInterface* Source, TSubclassOf<UObject> CubeClass, UWorld* World) override;
	virtual void DestroyCube() override;
	virtual void ChangeColor() override;
	virtual UStaticMeshComponent* GetStaticMeshComponent() override;
};
