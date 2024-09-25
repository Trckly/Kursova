// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthHandler.h"

#include "Kursova/Items/HealthItem.h"

bool UHealthHandler::Handle(AAbstractItem* PickUpItem)
{
	
	if(Cast<AHealthItem>(PickUpItem))
	{
		if(AddHealth.ExecuteIfBound(20))
		{
			UE_LOG(LogTemp, Warning, TEXT("Health item was used"));
			return true;
		}
		return false;
	}
	return UAbstractHandler::Handle(PickUpItem);
}
