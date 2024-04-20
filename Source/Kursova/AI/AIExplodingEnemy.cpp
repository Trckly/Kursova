// Fill out your copyright notice in the Description page of Project Settings.

#include "AIExplodingEnemy.h"
#include "MainPlayer.h"
#include "NetworkMessage.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AAIExplodingEnemy::AAIExplodingEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCapsuleComponent = GetCapsuleComponent();
}

// Called when the game starts or when spawned
void AAIExplodingEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerCapsuleComponent->OnComponentHit.AddDynamic(this, &AAIExplodingEnemy::OnHit);
}

// Called every frame
void AAIExplodingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIExplodingEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int AAIExplodingEnemy::DealDamage()
{
	return CharacterDamage;
}

int AAIExplodingEnemy::GetDamage(int Damage)
{
	CurrentHP -= Damage;
	CurrentHP = FMath::Clamp(CurrentHP, 0, MaxHP);

	return CurrentHP;
}

void AAIExplodingEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
	if(MainPlayer)
	{
		MainPlayer->GetDamage(DealDamage());
	}

	UE_LOG(LogTemp, Warning, TEXT("Collision with %s"), *OtherActor->GetName());
}

UBehaviorTree* AAIExplodingEnemy::GetBehaviourTree()
{
	return BehaviorTree;
}
