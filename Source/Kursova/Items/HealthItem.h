// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractItem.h"
#include "NiagaraActor.h"
#include "HealthItem.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class KURSOVA_API AHealthItem : public AAbstractItem
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComp;
	
public:
	AHealthItem();
	
	virtual void PostRegisterAllComponents() override;

	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;

	virtual AAbstractItem* CreatePocketCopy() override;

	static AHealthItem* CreateInstance(UWorld* World, TSubclassOf<AHealthItem> DerivedClass, FVector2D MapSize, FActorSpawnParameters const & SpawnParameters);
};
