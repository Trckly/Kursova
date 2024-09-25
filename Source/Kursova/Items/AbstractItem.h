// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraActor.h"
#include "AbstractItem.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API AAbstractItem : public ANiagaraActor
{
	GENERATED_BODY()
public:
	
	virtual void PostRegisterAllComponents() override;

	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;

	virtual AAbstractItem* CreatePocketCopy();
};
