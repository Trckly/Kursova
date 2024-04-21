// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBitingEnemy.h"

#include "MainPlayer.h"
#include "NetworkMessage.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAIBitingEnemy::AAIBitingEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCapsuleComponent = GetCapsuleComponent();
}

// Called when the game starts or when spawned
void AAIBitingEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerCapsuleComponent->OnComponentHit.AddDynamic(this, &AAIBitingEnemy::OnHit);
}

void AAIBitingEnemy::Die()
{
	if(!Destroy())
		UE_LOG(LogTemp, Warning, TEXT("Enemy haven't been destroyed after death!"));
}

// Called every frame
void AAIBitingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIBitingEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int AAIBitingEnemy::DealDamage()
{
	return CharacterDamage;
}

void AAIBitingEnemy::GetDamage(int Damage)
{
	if(CurrentHP <= 0) return;
	
	CurrentHP -= Damage;
	CurrentHP = FMath::Clamp(CurrentHP, 0, MaxHP);
	
	if(CurrentHP <= 0) Die();
}

void AAIBitingEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
	if(MainPlayer)
	{
		MainPlayer->GetDamage(DealDamage());
	}

	UE_LOG(LogTemp, Warning, TEXT("Collision with %s"), *OtherActor->GetName());
}

UBehaviorTree* AAIBitingEnemy::GetBehaviourTree()
{
	return BehaviorTree;
}


