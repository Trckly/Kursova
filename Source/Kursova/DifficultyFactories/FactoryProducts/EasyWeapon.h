// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "EasyWeapon.generated.h"

UCLASS()
class KURSOVA_API AEasyWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEasyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage = 1000;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	uint8 Capacity = 100;

public:	
	virtual void DetachFromActor(const FDetachmentTransformRules& DetachmentTransformRules) override;
	virtual void Destroy() override;
	virtual void SetActorEnableCollision(bool bNewActorEnableCollision) override;
	virtual void AttachToComponent(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules, FName SocketName) override;
};
