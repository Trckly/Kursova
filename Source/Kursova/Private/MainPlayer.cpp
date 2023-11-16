// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/HUD.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetRelativeLocation(CameraComponent->GetUpVector() * 90.f);
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	
	// PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AMainPlayer::Interact);
	PlayerInputComponent->BindAction(TEXT("Escape"), IE_Pressed, this, &AMainPlayer::ContinueGameplay);
}

void AMainPlayer::MoveForward(float Scale)
{
	if(Scale != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), Scale);
	}
}

void AMainPlayer::MoveRight(float Scale)
{
	if(Scale != 0.f)
	{
		AddMovementInput(GetActorRightVector(), Scale);
	}
}

void AMainPlayer::Interact()
{
	FHitResult HitResult;
	FVector StartTrace = CameraComponent->GetComponentLocation();
	FVector EndTrace = CameraComponent->GetComponentLocation() + CameraComponent->GetComponentRotation().Vector() * 150.f;
	
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartTrace, EndTrace, 50.f,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, { },
		EDrawDebugTrace::None, HitResult, true, FColor::Red, FColor::Green,
		1.f);

	if(HitResult.bBlockingHit)
	{
		PreviousActorLocation = GetActorLocation();
		PreviousActorRotation = CameraComponent->GetComponentRotation();

		SetActorLocation(WeaponChooseLocation, false, nullptr, ETeleportType::TeleportPhysics);
		SetActorRotation(WeaponChooseRotation);
		CameraComponent->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
		
		CameraComponent->bUsePawnControlRotation = false;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->DisableMovement();

		bShowCrosshair = false;
		bContinuable = true;
	}
}

void AMainPlayer::ContinueGameplay()
{
	SetActorLocation(PreviousActorLocation);
	CameraComponent->SetWorldRotation(PreviousActorRotation);

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	CameraComponent->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;
	
	bShowCrosshair = true;
	bContinuable = false;
}
