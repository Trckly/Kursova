// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "MainPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AEnemyController::AEnemyController()
{
	CustomPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	Sight->SightRadius = 1000.f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	
	CustomPerceptionComponent->ConfigureSense(*Sight);
	CustomPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("EnemyBehaviorTree"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("EnemyBlackBoard"));

	TeamId = FGenericTeamId(1);
	
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	CustomPerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AEnemyController::OnPerceptionUpdate);
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	IEnemyInterface* Enemy = Cast<IEnemyInterface>(InPawn);
	if(Enemy)
	{
		if(Enemy->GetBehaviourTree()->BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*Enemy->GetBehaviourTree()->BlackboardAsset);
			BehaviorTreeComponent->StartTree(*Enemy->GetBehaviourTree());
		}
	}
}

void AEnemyController::OnPerceptionUpdate(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	AMainPlayer* Player = Cast<AMainPlayer>(UpdateInfo.Target);

	if(Player)
	{
		BlackboardComponent->SetValueAsBool(FName("HasTarget"), UpdateInfo.Stimulus.WasSuccessfullySensed());
		BlackboardComponent->SetValueAsObject(FName("MainCharacter"), Player);
	}
}

ETeamAttitude::Type AEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	auto Player = Cast<AMainPlayer>(&Other);

	if(Player != nullptr)
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Neutral;
}
