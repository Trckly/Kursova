// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Kursova/Command/NegativeRotationCommand.h"
#include "Kursova/Command/PositiveRotationCommand.h"

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

	InputComponent->BindAction("RotatePositive",IE_Pressed, this, &AMainPlayerController::RotatePositive);
	InputComponent->BindAction("RotateNegative",IE_Pressed, this, &AMainPlayerController::RotateNegative);
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

void AMainPlayerController::RotatePositive()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACube::StaticClass(), OutActors);

	UPositiveRotationCommand* PositiveRotationCommand = NewObject<UPositiveRotationCommand>();
	if(PositiveRotationCommand)
	{
		PositiveRotationCommand->Execute(OutActors[0]);
	}
}

void AMainPlayerController::RotateNegative()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACube::StaticClass(), OutActors);

	UNegativeRotationCommand* NegativeRotationCommand = NewObject<UNegativeRotationCommand>();
	if(NegativeRotationCommand)
	{
		NegativeRotationCommand->Execute(OutActors[0]);
	}
}
