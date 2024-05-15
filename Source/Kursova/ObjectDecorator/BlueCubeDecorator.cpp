// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueCubeDecorator.h"

void ABlueCubeDecorator::BeginPlay()
{
}

ICubeInterface* ABlueCubeDecorator::CreateCube(ICubeInterface* Source, TSubclassOf<UObject> CubeClass, UWorld* World)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	auto NewCube =  World->SpawnActor<ABlueCubeDecorator>(CubeClass, FVector(391.f, -29.f, 173.f), FRotator::ZeroRotator, SpawnParameters);

	Wrap(NewCube);
	
	ChangeColor();
	
	return NewCube;
}

void ABlueCubeDecorator::DestroyCube()
{
	Super::DestroyCube();
}

void ABlueCubeDecorator::ChangeColor()
{
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(GetStaticMeshComponent()->GetMaterial(0), nullptr);

	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Blue);

	GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
}

UStaticMeshComponent* ABlueCubeDecorator::GetStaticMeshComponent()
{
	return Wrappee->GetStaticMeshComponent();
}
