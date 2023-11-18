// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kursova/MainUI/MainMenuWidget.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	
	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

	UFUNCTION()
	void SetCurrentPlayerInfo(FString PlayerName, FString PlayerCity);
};
