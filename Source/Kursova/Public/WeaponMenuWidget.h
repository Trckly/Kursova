// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeaponClass.h"
#include "WeaponDataWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kursova/UMG/WeaponEditWidget.h"
#include "WeaponMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UWeaponMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UVerticalBox* WeaponContent;

	UPROPERTY(meta=(BindWidget))
	UButton* SortButton;

	UPROPERTY(meta=(BindWidget))
	UButton* EditButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponDataWidget> WeaponDataWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponEditWidget> WeaponEditWidgetClass;

	UPROPERTY()
	TArray<UWeaponDataWidget*> ChildRowWidgets;

	UPROPERTY()
	UWeaponDataWidget* SelectedWidget;

	UPROPERTY()
	UWeaponEditWidget* EditWidget;

	UPROPERTY()
	TArray<AWeaponClass*> ActorPickedWeapons;

public:
	virtual void RemoveFromParent() override;
	
	UFUNCTION()
	void SortByCaliber();

	UFUNCTION()
	void CreatePropertiesEditor();

	UFUNCTION()
	void EditWeaponUnitProperties(const TArray<FText>& ChangedValues, const FString& OriginalModelName);

	UFUNCTION()
	void HandleSelectedRowWidget(const FString& ModelName);

	void ShellSort(TArray<UWeaponDataWidget*>& Children);

	void SetActorPickedWeapons(const TArray<AWeaponClass*>& PickedWeapons);
};

