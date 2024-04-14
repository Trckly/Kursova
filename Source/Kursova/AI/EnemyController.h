// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UEnemyInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API AEnemyController : public AAIController
{
	GENERATED_BODY()

	FGenericTeamId TeamId;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAIPerceptionComponent* CustomPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAISenseConfig_Sight* Sight;

public:
	AEnemyController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnPerceptionUpdate(const FActorPerceptionUpdateInfo& UpdateInfo);

	virtual FGenericTeamId GetGenericTeamId() const override{return TeamId;}

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
