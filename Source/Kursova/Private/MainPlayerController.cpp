// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

AMainPlayerController::AMainPlayerController()
{
	MouseSensivity = 50.f;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Get a reference to the player character
	ControlledPlayer = Cast<AMainPlayer>(GetPawn());

	// If the player character is not possessed yet
	if (!ControlledPlayer)
	{
		// Get the player character and possess it
		APawn* DefaultPawn = GetWorld()->SpawnActor<AMainPlayer>(AMainPlayer::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		if (DefaultPawn)
		{
			Possess(DefaultPawn);
			ControlledPlayer = Cast<AMainPlayer>(DefaultPawn);
		}
		else
		{
			UE_LOG(LogPlayerController, Error, TEXT("Failed to spawn or possess Main Player"));
		}
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind the LookUp axis input to the LookUp function in the player controller
	InputComponent->BindAxis("LookRight", this, &AMainPlayerController::LookRight);
	InputComponent->BindAxis("LookUp", this, &AMainPlayerController::LookUp);
}

void AMainPlayerController::LookUp(float Value)
{
	if(Value != 0 && ControlledPlayer)
	{
		float SensivityScalar = MouseSensivity * GetWorld()->GetDeltaSeconds();
		float PitchModulator = Value * SensivityScalar;
		
		ControlledPlayer->AddControllerPitchInput(PitchModulator);
	}
}

void AMainPlayerController::LookRight(float Value)
{
	if(Value != 0 && ControlledPlayer)
	{
		float SensivityScalar = MouseSensivity * GetWorld()->GetDeltaSeconds();
		ControlledPlayer->AddControllerYawInput(Value * SensivityScalar);
	}
}
