// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

// Sets default values
ACube::ACube()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMeshComponent;
}

void ACube::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* ACube::GetStaticMeshComponent()
{
	return StaticMeshComponent;
}

void ACube::ChangeColor()
{
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMeshComponent->GetMaterial(0), nullptr);

	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);

	StaticMeshComponent->SetMaterial(0, DynamicMaterial);
}

