// Fill out your copyright notice in the Description page of Project Settings.


#include "NegativeRotationCommand.h"

#include "Kursova/ObjectDecorator/Cube.h"

void UNegativeRotationCommand::Execute(AActor* Actor) const
{
	ACube* Cube = Cast<ACube>(Actor);
	if(Cube)
	{
		Cube->AddNegativeRotation();
	}
}
