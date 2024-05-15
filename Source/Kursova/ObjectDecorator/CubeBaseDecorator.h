// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeInterface.h"
#include "GameFramework/Actor.h"
#include "CubeBaseDecorator.generated.h"

UCLASS()
class KURSOVA_API ACubeBaseDecorator : public AActor, public ICubeInterface
{
	GENERATED_BODY()
	
public:	
	ACubeBaseDecorator();

protected:
	virtual void BeginPlay() override;
	
	ICubeInterface* Wrappee;

public:	
	void Wrap(ICubeInterface* Source);

	virtual ICubeInterface* CreateCube(ICubeInterface* Source, TSubclassOf<UObject> CubeClass, UWorld* World);
	virtual void DestroyCube() override;
	virtual void ChangeColor() override;
	virtual UStaticMeshComponent* GetStaticMeshComponent() override;
};
