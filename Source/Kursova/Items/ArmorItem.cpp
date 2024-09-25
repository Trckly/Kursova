// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorItem.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"

AArmorItem::AArmorItem()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetupAttachment(GetNiagaraComponent());
}

void AArmorItem::PostRegisterAllComponents()
{

}

bool AArmorItem::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	return false;
}

AAbstractItem* AArmorItem::CreatePocketCopy()
{
	return NewObject<AArmorItem>(GetWorld(), AArmorItem::StaticClass());
}

AArmorItem* AArmorItem::CreateInstance(UWorld* World, TSubclassOf<AArmorItem> DerivedClass, FVector2D MapSize,
                                       FActorSpawnParameters const& SpawnParameters)
{
	
	const FVector2D XBorders = FVector2D(MapSize.X / 2.f - MapSize.X, MapSize.X - MapSize.X / 2.f);
	const FVector2D YBorders = FVector2D(MapSize.Y / 2.f - MapSize.Y, MapSize.Y - MapSize.Y / 2.f);

	AArmorItem* Instance = nullptr;

	while (!Instance)
	{
		Instance = World->SpawnActor<AArmorItem>(DerivedClass, FVector(FMath::RandRange(XBorders.X, XBorders.Y),
		FMath::RandRange(YBorders.X, YBorders.Y), 92.f), FRotator(0.f, 0.f, 0.f),
		SpawnParameters);
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Created new Hardpoint"));

	return Instance;
}
