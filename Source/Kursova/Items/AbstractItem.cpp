// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractItem.h"

void AAbstractItem::PostRegisterAllComponents()
{

}

bool AAbstractItem::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	return false;
}

AAbstractItem* AAbstractItem::CreatePocketCopy()
{
	return this;
}
