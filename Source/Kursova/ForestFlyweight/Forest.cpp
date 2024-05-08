// Fill out your copyright notice in the Description page of Project Settings.


#include "Forest.h"

UForest::UForest()
{
	TreeFactory = NewObject<UTreeFactory>();
}

void UForest::PlantTree(FVector Position, const FString& TreeTypeName, TSubclassOf<ATree> TreeClass, UWorld* World)
{
	UTreeType* TreeType = TreeFactory->GetTreeType(TreeTypeName, TreeClass);

	ATree* NewTree = ATree::CreateTree(Position, TreeType, World);
	if(NewTree)
	{
		Trees.Emplace(NewTree);
	}
}
