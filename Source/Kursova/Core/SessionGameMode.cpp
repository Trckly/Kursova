// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGameMode.h"

void ASessionGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(MainMapBuilderClass)
	{
		MainMapBuilder = NewObject<UMainMapBuilder>(this, MainMapBuilderClass);

		if(MainMapBuilder)
		{
			MainMapBuilder->BuildFloor(FVector2D(10.f, 10.f));
		}
	}
}
