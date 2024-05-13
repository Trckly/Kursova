// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeInterface.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UCLASS()
class KURSOVA_API ACube : public AActor, public ICubeInterface
{
	GENERATED_BODY()
	
public:	
	ACube();

protected:
virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterial* DefaultMaterial;

public:
	static ICubeInterface* CreateCube(TSubclassOf<UObject> CubeClass, UWorld* World);
	virtual void DestroyCube() override;
	virtual UStaticMeshComponent* GetStaticMeshComponent() override;

	virtual void ChangeColor() override;
};
