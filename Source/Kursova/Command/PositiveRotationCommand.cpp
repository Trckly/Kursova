// Fill out your copyright notice in the Description page of Project Settings.


#include "PositiveRotationCommand.h"

#include "Kursova/ObjectDecorator/Cube.h"

void UPositiveRotationCommand::Execute(AActor* Actor) const
{
	ACube* Cube = Cast<ACube>(Actor);
	if(Cube)
	{
		Cube->AddPositiveRotation();
	}
}
