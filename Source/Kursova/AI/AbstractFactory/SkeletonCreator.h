// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IEnemyCreator.h"
#include "Kursova/AI/AIBitingEnemy.h"
#include "Kursova/AI/AIExplodingEnemy.h"
#include "UObject/NoExportTypes.h"
#include "SkeletonCreator.generated.h"

class ASessionGameMode;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KURSOVA_API USkeletonCreator : public UObject, public IIEnemyCreator
{
	GENERATED_BODY()
	
	FVector2D MapSize;

	FVector GetRandomLocation();
	
public:
	static USkeletonCreator* Create(ASessionGameMode* Owner, TSubclassOf<USkeletonCreator>const& GoblinCreatorClass, const FVector2D& Map);
	
	virtual IEnemyInterface* CreateEnemies(EEnemyType EnemyType) override;
	
	virtual IEnemyInterface* CreateBitingEnemies(FVector2D MapSizeRed) override;
	virtual IEnemyInterface* CreateShootingEnemies(FVector2D MapSizeRed) override;
	virtual IEnemyInterface* CreateExplodingEnemies(FVector2D MapSizeRed) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAIBitingEnemy> BitingSkeleton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAIBitingEnemy> ShootingSkeleton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAIExplodingEnemy> ExplodingSkeleton;
	
	UPROPERTY()
	TMap<EEnemyType, TScriptInterface<IEnemyInterface>> EnemyPrototypes;
};
