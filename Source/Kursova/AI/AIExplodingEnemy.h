// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "UEnemyInterface.h"
#include "GameFramework/Character.h"
#include "AIExplodingEnemy.generated.h"

UCLASS()
class KURSOVA_API AAIExplodingEnemy : public ACharacter, public IEnemyInterface, public IGenericTeamAgentInterface, public IEnemyObserver
{
	GENERATED_BODY()

	FGenericTeamId TeamId;

public:
	// Sets default values for this character's properties
	AAIExplodingEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAttacking = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int MaxHP = 100;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CharacterDamage  = 90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCapsuleComponent* PlayerCapsuleComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintCallable, Category=Character)
	virtual void Die() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category=Character)
	virtual int DealDamage() override;

	UFUNCTION(BlueprintCallable, Category=Character)
	virtual void GetDamage(int Damage) override;
	
	virtual IEnemyInterface* Clone(FVector Location) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	virtual FGenericTeamId GetGenericTeamId() const override{return TeamId;}

	virtual UBehaviorTree* GetBehaviourTree() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AAIExplodingEnemy> Self;

	virtual void Update(float Health) override;
};
