// Fill out your copyright notice in the Description page of Project Settings.


#include "InfiniteEnemyCreation.h"

void UInfiniteEnemyCreation::CreateEnemies(IIEnemyCreator* EnemyCreator, TArray<IEnemyInterface*>* Enemies)
{
	FTimerDelegate CreateDelegate = FTimerDelegate::CreateUObject( this, &UInfiniteEnemyCreation::Create, EnemyCreator, Enemies);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, CreateDelegate, 10.f, true, 5);

}

void UInfiniteEnemyCreation::Create(IIEnemyCreator* EnemyCreator, TArray<IEnemyInterface*>* Enemies)
{
	IEnemyInterface* NewEnemy = EnemyCreator->CreateEnemies(EEnemyType::Biting);
	if(NewEnemy)
	{
		Enemies->Add(NewEnemy);
		OnEnemyUpdated.ExecuteIfBound();
	}
	
	NewEnemy = EnemyCreator->CreateEnemies(EEnemyType::Exploding);
	if(NewEnemy)
	{
		Enemies->Add(NewEnemy);
		OnEnemyUpdated.ExecuteIfBound();
	}
	
}
