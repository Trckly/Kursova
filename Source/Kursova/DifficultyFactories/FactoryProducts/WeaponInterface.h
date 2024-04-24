// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KURSOVA_API IWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void DetachFromActor(const FDetachmentTransformRules& DetachmentTransformRules) = 0;
	virtual void Destroy() = 0;
	virtual void SetActorEnableCollision(bool bNewActorEnableCollision) = 0;
	virtual void AttachToComponent(USceneComponent* Parent,	const FAttachmentTransformRules& AttachmentRules,
		FName SocketName = NAME_None) = 0;
};
