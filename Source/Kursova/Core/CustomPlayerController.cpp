// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

#include "MainPlayer.h"

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);

		if(MainMenuWidget)
		{
			MainMenuWidget->SetUserInfo.BindDynamic(this, &ACustomPlayerController::SetCurrentPlayerInfo);
			MainMenuWidget->AddToViewport();
			
			SetShowMouseCursor(true);
			SetInputMode(FInputModeGameAndUI());
		}
	}
}

void ACustomPlayerController::SetCurrentPlayerInfo(FString PlayerName, FString PlayerCity)
{
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(GetCharacter());

	if(MainPlayer)
	{
		MainPlayer->SetName(PlayerName);
		MainPlayer->SetCity(PlayerCity);
		
		MainMenuWidget->RemoveFromViewport();
		//Destroy(MainMenuWidget);
		
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get Player in CustomPlayerController"));
	}
}
