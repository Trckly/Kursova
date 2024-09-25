// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorHandler.h"

#include "Kursova/Items/ArmorItem.h"

bool UArmorHandler::Handle(AAbstractItem* PickUpItem)
{
	if(Cast<AArmorItem>(PickUpItem))
	{
		if(AddArmor.ExecuteIfBound(20))
		{
			UE_LOG(LogTemp, Warning, TEXT("Armor item was used"));
			return true;
		}
		return false;
	}
	return UAbstractHandler::Handle(PickUpItem);
}
