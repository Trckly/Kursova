// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthItem.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"

AHealthItem::AHealthItem()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetupAttachment(GetNiagaraComponent());
}

void AHealthItem::PostRegisterAllComponents()
{

}

bool AHealthItem::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	return false;
}

AAbstractItem* AHealthItem::CreatePocketCopy()
{
	return NewObject<AHealthItem>(GetWorld(), StaticClass());
}

AHealthItem* AHealthItem::CreateInstance(UWorld* World, TSubclassOf<AHealthItem> DerivedClass, FVector2D MapSize,
                                         FActorSpawnParameters const& SpawnParameters)
{
	const FVector2D XBorders = FVector2D(MapSize.X / 2.f - MapSize.X, MapSize.X - MapSize.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSize.Y / 2.f - MapSize.Y, MapSize.Y - MapSize.Y / 2.f);

	AHealthItem* Instance = nullptr;

	while (!Instance)
	{
		Instance = World->SpawnActor<AHealthItem>(DerivedClass, FVector(FMath::RandRange(XBorders.X, XBorders.Y),
		FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f),
		SpawnParameters);
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Created new Hardpoint"));

	return Instance;
}
