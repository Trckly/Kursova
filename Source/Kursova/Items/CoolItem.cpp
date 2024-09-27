// Fill out your copyright notice in the Description page of Project Settings.


#include "CoolItem.h"
#include "NiagaraComponent.h"
#include "MainPlayer.h"

ACoolItem* ACoolItem::Instance = nullptr;

ACoolItem::ACoolItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetupAttachment(GetNiagaraComponent());

	CaptureScoreBar = 0;

	
}

void ACoolItem::BeginPlay()
{
	Super::BeginPlay();

	if(SphereComp)
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACoolItem::OnBeginOverlap);
		SphereComp->OnComponentEndOverlap.AddDynamic(this, &ACoolItem::OnEndOverlap);
	}
}

void ACoolItem::PostRegisterAllComponents()
{
	
}

bool ACoolItem::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	return false;
}

ACoolItem* ACoolItem::GetInstance(UWorld* World, TSubclassOf<ACoolItem> DerivedClass, FVector2D MapSize, FActorSpawnParameters const & SpawnParameters)
{
	

	const FVector2D XBorders = FVector2D(MapSize.X / 2.f - MapSize.X, MapSize.X - MapSize.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSize.Y / 2.f - MapSize.Y, MapSize.Y - MapSize.Y / 2.f);
	
	if(Instance == nullptr)
	{
		// Error here. It creates instance and if location happens to be invalid it can't create another one 
		Instance = World->SpawnActor<ACoolItem>(DerivedClass, FVector(FMath::RandRange(XBorders.X, XBorders.Y), FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f), SpawnParameters);
		UE_LOG(LogTemp, Warning, TEXT("Created new Hardpoint"));
	}
	
	return Instance;
}

void ACoolItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if(Cast<AMainPlayer>(OtherActor))
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACoolItem::IncreaseScore, 0.3f, true);
}

void ACoolItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if(Cast<AMainPlayer>(OtherActor))
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
}

void ACoolItem::IncreaseScore()
{
	CaptureScoreBar += 1;
	CaptureScoreBar = FMath::Clamp(CaptureScoreBar, 0, 100);
	UE_LOG(LogTemp, Warning, TEXT("Hardpoint Score is: %d"), CaptureScoreBar);
	
	if(CaptureScoreBar >= 100)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hardpoint was captured"));
		SphereComp->OnComponentBeginOverlap.Clear();
		SphereComp->OnComponentEndOverlap.Clear();

		ScoreNotify.ExecuteIfBound();
	}
}

bool ACoolItem::ClearInstance()
{
	if(Instance)
	{
		Instance->ScoreNotify.Clear();
		Instance->Destroy();
		Instance = nullptr;
		return true;
	}
	return false;
}

