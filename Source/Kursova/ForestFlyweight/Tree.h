// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeType.h"
#include "GameFramework/Actor.h"
#include "Tree.generated.h"

UCLASS()
class KURSOVA_API ATree : public AActor
{
	GENERATED_BODY()
	
public:	
	ATree();
	
	static ATree* CreateTree(FVector WorldPosition, UTreeType* Type, UWorld* World);
	static ATree* CreateTree(float X, float Y, float Z, UTreeType* Type, UWorld* World);

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;
	
	FVector Position;

	UPROPERTY()
	UTreeType* TreeType;

protected:
	void Initialize(FVector WorldPosition, UTreeType* Type);

public:
	
};
