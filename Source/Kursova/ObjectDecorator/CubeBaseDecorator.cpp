// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeBaseDecorator.h"

// Sets default values
ACubeBaseDecorator::ACubeBaseDecorator()
{
}

void ACubeBaseDecorator::Wrap(ICubeInterface* Source)
{
	Wrappee = Source;
}

void ACubeBaseDecorator::ChangeColor()
{
	Wrappee->ChangeColor();
}

UStaticMeshComponent* ACubeBaseDecorator::GetStaticMeshComponent()
{
	return Wrappee->GetStaticMeshComponent();
}

