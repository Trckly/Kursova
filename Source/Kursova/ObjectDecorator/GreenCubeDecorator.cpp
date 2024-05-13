// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenCubeDecorator.h"

void AGreenCubeDecorator::BeginPlay()
{
}

ICubeInterface* AGreenCubeDecorator::CreateCube(ICubeInterface* Source, TSubclassOf<UObject> CubeClass, UWorld* World)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	auto NewCube =  World->SpawnActor<AGreenCubeDecorator>(CubeClass, FVector(391.f, -29.f, 173.f), FRotator::ZeroRotator, SpawnParameters);

	Wrap(NewCube);
	
	ChangeColor();
	
	return NewCube;
}

void AGreenCubeDecorator::DestroyCube()
{
	Super::DestroyCube();
}

void AGreenCubeDecorator::ChangeColor()
{
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(GetStaticMeshComponent()->GetMaterial(0), nullptr);

	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Green);

	GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
}

UStaticMeshComponent* AGreenCubeDecorator::GetStaticMeshComponent()
{
	return Wrappee->GetStaticMeshComponent();
}
