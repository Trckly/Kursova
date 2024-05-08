// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeType.h"

UTreeType::UTreeType()
{
}

// Static creation function implementation
UTreeType* UTreeType::CreateTreeType(const FString& Name, TSubclassOf<UObject> Class)
{
	UTreeType* NewTreeType = NewObject<UTreeType>();
	if (NewTreeType)
	{
		NewTreeType->Initialize(Name, Class);
	}
	return NewTreeType;
}

// Initialization function implementation
void UTreeType::Initialize(const FString& Name, TSubclassOf<UObject> Class)
{
	TreeTypeName = Name;
	TreeClass = Class;
}

TSubclassOf<UObject> UTreeType::GetTreeClass()
{
	return TreeClass;
}

FString UTreeType::GetName()
{
	return TreeTypeName;
}
