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

void AAIExplodingEnemy::Die()
{
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(MainPlayer)
	{
		MainPlayer->Detach(this);
	}
	
	if(!Destroy())
		UE_LOG(LogTemp, Warning, TEXT("Enemy haven't been destroyed after death!"));
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

void AAIExplodingEnemy::GetDamage(int Damage)
{
	if(CurrentHP <= 0) return;
	
	CurrentHP -= Damage;
	CurrentHP = FMath::Clamp(CurrentHP, 0, MaxHP);
	
	if(CurrentHP <= 0) Die();
}

IEnemyInterface* AAIExplodingEnemy::Clone(FVector Location)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	return GetWorld()->SpawnActor<IEnemyInterface>(Self, Location, FRotator(0.f, 0.f, 0.f), SpawnInfo);
}

void AAIExplodingEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
	if(MainPlayer)
	{
		MainPlayer->GetDamage(DealDamage());
		Die();
	}

	UE_LOG(LogTemp, Warning, TEXT("Collision with %s"), *OtherActor->GetName());
}

UBehaviorTree* AAIExplodingEnemy::GetBehaviourTree()
{
	return BehaviorTree;
}

void AAIExplodingEnemy::Update(float Health)
{
	if(Health <= 50)
		CharacterDamage = 40;
	else
		CharacterDamage = 90;
}
