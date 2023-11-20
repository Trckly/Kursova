// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "PlayerPanelWidget.generated.h"

class AMainPlayer;

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

	UPROPERTY()
	AMainPlayer* Player;
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetWidget(AMainPlayer const * MainPlayer);
};
