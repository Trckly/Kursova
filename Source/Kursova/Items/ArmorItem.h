// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractItem.h"
#include "ArmorItem.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class KURSOVA_API AArmorItem : public AAbstractItem
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComp;
	
public:
	AArmorItem();
	
	virtual void PostRegisterAllComponents() override;

	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;

	virtual AAbstractItem* CreatePocketCopy() override;
	
	static AArmorItem* CreateInstance(UWorld* World, TSubclassOf<AArmorItem> DerivedClass, FVector2D MapSize, FActorSpawnParameters const & SpawnParameters);

};
