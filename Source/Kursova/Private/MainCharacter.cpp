// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(55.f, 96.0f);
	RootComponent= CapsuleComponent;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	CameraComponent->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
}

void AMainCharacter::MoveForward(float Scale)
{
	if(Scale != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), Scale);
	}
}

void AMainCharacter::MoveRight(float Scale)
{
	if(Scale != 0.f)
	{
		AddMovementInput(GetActorRightVector(), Scale);
	}
}
