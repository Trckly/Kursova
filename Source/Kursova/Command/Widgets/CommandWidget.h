// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CommandWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FPositiveRotationDelegate, AActor*, Actor);
DECLARE_DYNAMIC_DELEGATE_OneParam(FNegativeRotationDelegate, AActor*, Actor);
DECLARE_DYNAMIC_DELEGATE(FWidgetClosed);

UCLASS()
class KURSOVA_API UCommandWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UButton* PosRotationButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UButton* NegRotationButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UButton* RedStateButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UButton* GreenStateButton;

	UPROPERTY()
	AActor* Actor;

public:
	UFUNCTION()
	void PositiveRotation();

	UFUNCTION()
	void NegativeRotation();

	UFUNCTION()
	void RedState();
	
	UFUNCTION()
	void GreenState();
	
	void CloseWidget();

	void PassActor(AActor* HitActor);

public:
	FPositiveRotationDelegate PositiveRotationDelegate;
	FNegativeRotationDelegate NegativeRotationDelegate;
	FWidgetClosed WidgetClosed;
};
