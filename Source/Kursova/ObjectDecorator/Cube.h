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
	// Sets default values for this actor's properties
	ACube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterial* DefaultMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UStaticMeshComponent* GetStaticMeshComponent() override;

	virtual void ChangeColor() override;
};
