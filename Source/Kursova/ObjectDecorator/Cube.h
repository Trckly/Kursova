// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kursova/Observer/SubscriptionManager.h"
#include "Kursova/State/State.h"
#include "Cube.generated.h"

UCLASS()
class KURSOVA_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	ACube();

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterial* DefaultMaterial;

	bool bPositiveRotation = false;
	bool bNegativeRotation = false;

	float RotationSpeed = 100.f;
	
	IState* State;

	UPROPERTY()
	USubscriptionManager* SubscriptionManager;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;

public:
	UStaticMeshComponent* GetStaticMeshComponent();

	void ChangeState(IState* otherState);

	void AddPositiveRotation();
	void AddNegativeRotation();

	UMaterialInstanceDynamic* GetDynamicMaterial();
	void SetDynamicMaterial(UMaterialInstanceDynamic* Material);
};
