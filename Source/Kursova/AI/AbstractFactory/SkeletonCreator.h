// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IEnemyCreator.h"
#include "Kursova/AI/AIBitingEnemy.h"
#include "UObject/NoExportTypes.h"
#include "SkeletonCreator.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KURSOVA_API USkeletonCreator : public UObject, public IIEnemyCreator
{
	GENERATED_BODY()
	
public:
	
	virtual IEnemyInterface* CreateBitingEnemies() override;
	virtual IEnemyInterface* CreateShootingEnemies() override;
	virtual IEnemyInterface* CreateExplodingEnemies() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAIBitingEnemy> BitingSkeleton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAIBitingEnemy> ShootingSkeleton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAIBitingEnemy> ExplodingSkeleton;
};
