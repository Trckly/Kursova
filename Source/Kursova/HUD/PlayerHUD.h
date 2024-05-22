// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* PHealth;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* PArmor;
	
public:

	UFUNCTION()
	void SetHealth(float Health);
	
	UFUNCTION()
	void SetArmor(float Armor);
};
