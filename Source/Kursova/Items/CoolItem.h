// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraActor.h"
#include "Components/SphereComponent.h"
#include "CoolItem.generated.h"

DECLARE_DYNAMIC_DELEGATE(FScoreNotify);
/**
 * 
 */
UCLASS()
class KURSOVA_API ACoolItem : public ANiagaraActor
{
	GENERATED_BODY()
	
	int CaptureScoreBar;
	
	static ACoolItem* Instance;

	FTimerHandle TimerHandle;

	ACoolItem(const FObjectInitializer& ObjectInitializer);
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComp;
	
public:
	
	virtual void BeginPlay() override;
	
	virtual void PostRegisterAllComponents() override;

	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
	
	static ACoolItem* GetInstance(UWorld* World, TSubclassOf<ACoolItem> DerivedClass, FVector2D MapSize, FActorSpawnParameters const & SpawnParameters);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int OtherBodyIndex);
	
	UFUNCTION()
	void IncreaseScore();

	static bool ClearInstance();

	UPROPERTY()
	FScoreNotify ScoreNotify;
};