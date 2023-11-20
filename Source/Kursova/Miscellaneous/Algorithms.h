// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Algorithms.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UAlgorithms : public UObject
{
	GENERATED_BODY()
public:
	template<typename T>
	void ShellSort(TArray<T>& Array);
	
};

template <typename T>
void UAlgorithms::ShellSort(TArray<T>& Array)
{
	for (int Gap = Array.Num() / 2; Gap > 0; Gap /= 2)
	{
		for (int i = Gap; i < Array.Num(); ++i)
		{
			float Temp = Array[i];

			int j;
			for(j = i; j >= Gap && Array[j-Gap] > Temp; j -= Gap)
				Array[j] = Array[j-Gap];

			Array[j] = Temp;
		}
	}
}
