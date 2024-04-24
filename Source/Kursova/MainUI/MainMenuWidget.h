// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "MainMenuWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FSetUserInfo, FString const&, PlayerName, FString const&, PlayerCity);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSetDifficultyMode, const FString&, DifficultyMode);


/**
 * 
 */
UCLASS()
class KURSOVA_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UEditableText* EName;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UEditableText* ECity;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TNameError;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TCityError;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BConfirm;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxString* CBDifficulty;

public:
	UPROPERTY()
	FSetUserInfo SetUserInfo;

	UPROPERTY()
	FSetDifficultyMode SetDifficultyMode;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void ConfirmRegistration();
};
