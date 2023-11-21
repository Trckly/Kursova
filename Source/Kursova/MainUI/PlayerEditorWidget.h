// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "PlayerEditorWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnCloseButtonClicked);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnSaveButtonClicked, bool, PCanMove, bool, PCanJump, bool, PCanFire);

/**
 * 
 */
UCLASS()
class KURSOVA_API UPlayerEditorWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCheckBox* CCanMove;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCheckBox* CCanJump;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UCheckBox* CCanFire;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BClose;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BSave;

public:
	virtual void NativeConstruct() override;

	FOnCloseButtonClicked OnCloseButtonClicked;

	FOnSaveButtonClicked OnSaveButtonClicked;

	UFUNCTION()
	void SetBehaviorStats(bool CanMove, bool CanJump, bool CanFire);
	
	UFUNCTION()
	void Exit();

	UFUNCTION()
	void SaveStats();
};
