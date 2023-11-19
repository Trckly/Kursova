// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"

#include "AWeaponClass.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kursova/UMG/CrosshairWidget.h"

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

	///
	/// Server Logic
	///
	APlayerController* PlayerController = GetController<APlayerController>();
	if(ServerWidgetClass)
	{
		if(PlayerController)
		{
			ServerWidget = CreateWidget<UServerWidget>(PlayerController, ServerWidgetClass);
			
			if(ServerWidget)
			{
				ServerWidget->SetServerSettings.BindDynamic(this, &AMainPlayer::CreateSession);
				ServerWidget->OnFindSession.BindDynamic(this, &AMainPlayer::ConnectToService);
				ServerWidget->OnJoinToSession.BindDynamic(this, &AMainPlayer::JoinSession);
			}
		}
	}

	///
	///	Creating crosshair
	///
	auto CreatedCrosshair = CreateWidget<UCrosshairWidget>(GetWorld(), CrosshairWidgetClass);
	if(CreatedCrosshair)
	{
		CrosshairWidget = CreatedCrosshair;
		CrosshairWidget->AddToViewport();
	}
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
	
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AMainPlayer::Interact);
	PlayerInputComponent->BindAction(TEXT("Escape"), IE_Pressed, this, &AMainPlayer::ContinueGameplay);

	///
	/// Server Logic
	///
	PlayerInputComponent->BindAction("OpenServerMenu", IE_Pressed, this, &AMainPlayer::AddServerWidgetToViewPort);
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
		EDrawDebugTrace::ForDuration	, HitResult, true, FColor::Red, FColor::Green,
		1.f);

	if(HitResult.bBlockingHit)
	{
		AActor* ReturnedActor = HitResult.GetActor();
		if(Cast<AWeaponClass>(ReturnedActor))
		{
			ProcessHitWeapon(Cast<AWeaponClass>(ReturnedActor));
		}
		else
		{
			ProcessHitRack();
		}
	}
}

void AMainPlayer::ContinueGameplay()
{
	if(bContinuable)
	{
		SetActorLocation(PreviousActorLocation);
		CameraComponent->SetWorldRotation(PreviousActorRotation);

		GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		CameraComponent->bUsePawnControlRotation = true;
		bUseControllerRotationYaw = true;

		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		
		bContinuable = false;

		///
		/// Widgets Handling
		///
		CrosshairWidget->AddToViewport();

		WeaponMenuWidget->RemoveFromParent();
	}
}

void AMainPlayer::ProcessHitRack()
{
	PreviousActorLocation = GetActorLocation();
	PreviousActorRotation = CameraComponent->GetComponentRotation();

	SetActorLocation(WeaponChooseLocation, false, nullptr, ETeleportType::TeleportPhysics);
	SetActorRotation(WeaponChooseRotation);
	CameraComponent->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
		
	CameraComponent->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->DisableMovement();

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	bContinuable = true;
	
	CrosshairWidget->RemoveFromViewport();
	auto CreatedWeaponMenu = CreateWidget<UWeaponMenuWidget>(GetWorld(), WeaponMenuWidgetClass);
	if(CreatedWeaponMenu)
	{
		WeaponMenuWidget = CreatedWeaponMenu;
		WeaponMenuWidget->SetActorPickedWeapons(PickedWeapons);
		WeaponMenuWidget->AddToViewport();
	}
}

void AMainPlayer::ProcessHitWeapon(AWeaponClass* WeaponActor)
{
	WeaponActor->Destroy();
	PickedWeapons.Push(WeaponActor);
}

TArray<AWeaponClass*> AMainPlayer::GetAllPickedWeapons()
{
	return PickedWeapons;
}

///
/// Server Logic
///

void AMainPlayer::CreateSession(FString Name, bool Privacy, FString Password, int InputNumberOfPlayers)
{
	if(ServerWidget && ServerWidget->IsInViewport())
	{
		ServerWidget->RemoveFromViewport();
	}
	GetController<APlayerController>()->SetShowMouseCursor(false);
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());

	CreateS(InputNumberOfPlayers, Name, Privacy, Password);
}

void AMainPlayer::AddServerWidgetToViewPort()
{
	if(ServerWidget)
	{
		ServerWidget->AddToViewport();
		
		GetController<APlayerController>()->SetShowMouseCursor(true);
		GetController<APlayerController>()->SetInputMode(FInputModeUIOnly());
		
	}
}

void AMainPlayer::ConnectToService()
{
	FindS();
}

void AMainPlayer::JoinSession(FBlueprintSessionResult SessionResult, const FString& Password)
{
	if (ServerWidget && ServerWidget->IsInViewport())
	{
		ServerWidget->RemoveFromViewport();
	}
	GetController<APlayerController>()->SetShowMouseCursor(false);
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());
	OnCharacterJoinSession.Broadcast(SessionResult, Password);
}

void AMainPlayer::SetSessionsToWidget(TArray<FBlueprintSessionResult> BlueprintSessionResults)
{
	if(ServerWidget && ServerWidget->IsInViewport())
	{
		ServerWidget->SetSessions(BlueprintSessionResults);
	}
}
