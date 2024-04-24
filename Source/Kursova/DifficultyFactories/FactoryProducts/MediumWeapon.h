// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "MediumWeapon.generated.h"

UCLASS()
class KURSOVA_API AMediumWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMediumWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage = 500;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	uint8 Capacity = 50;

public:	
	virtual auto DetachFromActor(const FDetachmentTransformRules& DetachmentTransformRules) -> void override;
	virtual void Destroy() override;
	virtual void SetActorEnableCollision(bool bNewActorEnableCollision) override;
	virtual void AttachToComponent(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules, FName SocketName) override;
};
