// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"

ATree::ATree()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
}

ATree* ATree::CreateTree(FVector WorldPosition, UTreeType* Type, UWorld* World)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TSubclassOf<UObject> TreeClass = Type->GetTreeClass();
	
	ATree* NewTree = World->SpawnActor<ATree>(TreeClass, FVector::ZeroVector, FRotator(0.f, FMath::RandRange(0, 360), 0.f), SpawnParameters);
	if(NewTree)
	{
		NewTree->Initialize(WorldPosition, Type);
		return NewTree;
	}
	
	return nullptr;
}

ATree* ATree::CreateTree(float X, float Y, float Z, UTreeType* Type, UWorld* World)
{
	return CreateTree(FVector(X, Y, Z), Type, World);
}

void ATree::Initialize(FVector WorldPosition, UTreeType* Type)
{
	Position = WorldPosition;
	
	SetActorLocation(Position);
}

