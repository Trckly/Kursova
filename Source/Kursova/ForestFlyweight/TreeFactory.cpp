// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeFactory.h"

UTreeType* UTreeFactory::GetTreeType(const FString& Name, TSubclassOf<UObject> TreeClass)
{
	for(int i = 0; i < TreeTypes.Num(); ++i)
	{
		if(TreeTypes[i]->GetName() == Name && TreeTypes[i]->GetTreeClass() == TreeClass)
		{
			return TreeTypes[i];
		}
	}

	UTreeType* NewTreeType = UTreeType::CreateTreeType(Name, TreeClass);
	if(NewTreeType)
	{
		TreeTypes.Emplace(NewTreeType);
		return NewTreeType;
	}
	return nullptr;
}
