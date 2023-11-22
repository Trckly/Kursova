// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "PlayerPanelWidget.generated.h"

class AMainPlayer;

DECLARE_DYNAMIC_DELEGATE_OneParam(FSetWidgetActive, int, PIndex);

/**
 * 
 */
UCLASS()
class KURSOVA_API UPlayerPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TName;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TCity;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* THitPoints;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCheckBox* CIsInGodMode;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BSetActive;

	UPROPERTY()
	int PlayerIndex;

	UPROPERTY()
	AMainPlayer* Player;

	UPROPERTY()
	FLinearColor StandardBackgroundColor;
	
public:
	UPROPERTY()
	FSetWidgetActive SetWidgetActive;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetWidget(AMainPlayer* MainPlayer, int Index);

	UFUNCTION()
	FString GetName();

	UFUNCTION()
	FString GetCity();

	UFUNCTION()
	int GetIndex();

	UFUNCTION()
	void SetActive();

	UFUNCTION()
	void SetGodMode(bool IsGodModeSet);

	UFUNCTION()
	void SetBehavior(bool CanMove, bool CanJump, bool CanFire);

	UFUNCTION()
	bool GetAbilityToMove();

	UFUNCTION()
	bool GetAbilityToJump();

	UFUNCTION()
	bool GetAbilityToFire();

	UFUNCTION()
	void ResetBackgroundColor();

	UFUNCTION()
	void SetBackgroundColor();
};
