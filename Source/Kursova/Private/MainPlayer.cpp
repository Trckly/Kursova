// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "../ServerLogic/UI/ServerWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kursova/MainUI/MainMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kursova/Core/CustomPlayerController.h"
#include "Net/UnrealNetwork.h"
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

	IsInGodMode = false;
	BehaviorSet = {true, true, true};
	Health = 100.f;
	bReplicates = true;
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
	
	ACustomPlayerController* PController = Cast<ACustomPlayerController>(GetOwner());
	if(PController)
	{
		if(MainMenuWidgetClass)
		{
			MainMenuWidget = CreateWidget<UMainMenuWidget>(PController, MainMenuWidgetClass);

			if(MainMenuWidget)
			{
				MainMenuWidget->SetUserInfo.BindDynamic(this, &AMainPlayer::SetNameAndCity);
				MainMenuWidget->AddToViewport();
			
				PController->SetShowMouseCursor(true);
				PController->SetInputMode(FInputModeGameAndUI());
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
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);

	///
	/// Server Logic
	///
	PlayerInputComponent->BindAction("OpenServerMenu", IE_Pressed, this, &AMainPlayer::AddServerWidgetToViewPort);
}

void AMainPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsInGodMode)
	{
		Jump();
	}
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

///
/// Andrii Kursova
///
void AMainPlayer::SetName(FString Name)
{
	SPlayerName = Name;
}

FString AMainPlayer::GetPlayerName() const
{
	return SPlayerName;
}

void AMainPlayer::SetCity(FString City)
{
	SCity = City;
}

FString AMainPlayer::GetCity() const
{
	return SCity;
}

FBehaviorSet AMainPlayer::GetBehaviorSet() const
{
	return BehaviorSet;
}

bool AMainPlayer::GetGodModeState() const
{
	return IsInGodMode;
}

void AMainPlayer::SetGodModeState(bool HasGodMode)
{
	IsInGodMode = HasGodMode;
}

float AMainPlayer::GetHealth() const
{
	return Health;
}

int AMainPlayer::GetPlayerIndex() const
{
	return PlayerIndex;
}

void AMainPlayer::SetPlayerIndex(int Index)
{
	PlayerIndex = Index;
}

void AMainPlayer::ServerSetNameAndCity_Implementation(FString const& Name, FString const& City)
{
	MulticastSetNameAndCity(Name, City);
}
void AMainPlayer::MulticastSetNameAndCity_Implementation(FString const& Name, FString const& City)
{
	SPlayerName = Name;
	SCity = City;
	
}

void AMainPlayer::SetNameAndCity(FString const& Name, FString const& City)
{
	ServerSetNameAndCity(Name, City);
	
	ACustomPlayerController* PController = Cast<ACustomPlayerController>(GetOwner());
	if(PController)
	{
    	
		MainMenuWidget->RemoveFromViewport();
    		
		PController->SetShowMouseCursor(false);
		PController->SetInputMode(FInputModeGameOnly());
	}
}

void AMainPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainPlayer, Health);
	DOREPLIFETIME(AMainPlayer, SPlayerName);
	DOREPLIFETIME(AMainPlayer, SCity);
	DOREPLIFETIME(AMainPlayer, IsInGodMode);
	DOREPLIFETIME(AMainPlayer, BehaviorSet);
}

void AMainPlayer::Multicast_SetGodMode_Implementation(bool IsGodModeSet)
{
	IsInGodMode = IsGodModeSet;
}

void AMainPlayer::Multicast_SetBehavior_Implementation(bool CanMove, bool CanJump, bool CanFire)
{
	BehaviorSet.CanMove = CanMove;
	BehaviorSet.CanJump = CanJump;
	BehaviorSet.CanShoot = CanFire;
}
