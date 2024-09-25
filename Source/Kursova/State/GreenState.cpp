// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenState.h"
#include "Kursova/ObjectDecorator/Cube.h"

void UGreenState::ChangeColor(AActor* Actor)
{
	ACube* Cube = Cast<ACube>(Actor);
	if(Cube)
	{
		if(!Cube->GetDynamicMaterial())
		{
			UMaterialInstanceDynamic* DynamicMaterial =
				UMaterialInstanceDynamic::Create(Cube->GetStaticMeshComponent()->GetMaterial(0), nullptr);

			Cube->SetDynamicMaterial(DynamicMaterial);
		}
		
		Cube->GetDynamicMaterial()->SetVectorParameterValue(TEXT("Color"), FLinearColor::Green);

		Cube->GetStaticMeshComponent()->SetMaterial(0, Cube->GetDynamicMaterial());
	}
}
