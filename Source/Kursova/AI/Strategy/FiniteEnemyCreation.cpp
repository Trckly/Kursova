// Fill out your copyright notice in the Description page of Project Settings.


#include "FiniteEnemyCreation.h"

void UFiniteEnemyCreation::CreateEnemies(IIEnemyCreator* EnemyCreator, TArray<IEnemyInterface*>* Enemies)
{	
	while(NumberOfBiters--)
	{
		IEnemyInterface* NewEnemy = EnemyCreator->CreateEnemies(EEnemyType::Biting);
		if(NewEnemy)
		{
			Enemies->Add(NewEnemy);
			OnEnemyUpdated.ExecuteIfBound();
		}
		else
			NumberOfBiters++;
	}
	while(NumberOfSuicidal--)
	{
		IEnemyInterface* NewEnemy = EnemyCreator->CreateEnemies(EEnemyType::Exploding);
		if(NewEnemy)
		{
			Enemies->Add(NewEnemy);
			OnEnemyUpdated.ExecuteIfBound();
		}
		else
			NumberOfSuicidal++;		
	}
}
