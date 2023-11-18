// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

public:
	UFUNCTION()
	void SortByCaliber();

	UFUNCTION()
	void EditProperties();

	void ShellSort(TArray<UWeaponDataWidget*>& Children);
};

