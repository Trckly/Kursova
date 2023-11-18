// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "MainMenuWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FSetUserInfo, FString, PlayerName, FString, PlayerCity);

/**
 * 
 */
UCLASS()
class KURSOVA_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

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

	UFUNCTION()
	void ConfirmRegistration();

	UPROPERTY()
	FSetUserInfo SetUserInfo;
	
};
