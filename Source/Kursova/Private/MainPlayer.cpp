// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "../ServerLogic/UI/ServerWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	CameraComponent->SetupAttachment(GetMesh(), "head");

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

	PlayerWeaponSocketsName = {"AWM_Attach", "AK47_Attach", "M16A4_Attach", "M870_Attach", "HK416_Attach"};
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
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AMainPlayer::Shoot);

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
		if(WeaponMenuWidget->GetSelectedWidget())
		{
			CreateWeaponAttach(WeaponMenuWidget->GetSelectedWidget()->GetModelName());
		}
		
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
	WeaponActor->SetHidden(true);
	WeaponActor->SetActorLocation(WeaponActor->GetActorLocation() + WeaponActor->GetActorUpVector() * -1000.f, false);
	PickedWeapons.Push(WeaponActor);
	CreateWeaponAttach(WeaponActor);
}

TArray<AWeaponClass*> AMainPlayer::GetAllPickedWeapons()
{
	return PickedWeapons;
}

void AMainPlayer::Shoot()
{
	FHitResult HitResult;
	FVector StartTrace = CameraComponent->GetComponentLocation();
	FVector EndTrace = CameraComponent->GetComponentLocation() + CameraComponent->GetComponentRotation().Vector() * 10000.f;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTrace, EndTrace,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, {this},
		EDrawDebugTrace::ForDuration, HitResult, true, FColor::Red, FColor::Green,
		2.f);
}

void AMainPlayer::CreateWeaponAttach(AWeaponClass* WeaponActor)
{
	if(PlayerWeaponSocketsName[AWM].Contains(WeaponActor->GetStructure().Model))
	{
		AttachAWM();
	}
	if(PlayerWeaponSocketsName[AK47].Contains(WeaponActor->GetStructure().Model))
	{
		AttachAK47();
	}
	if(PlayerWeaponSocketsName[M16A4].Contains(WeaponActor->GetStructure().Model))
	{
		AttachM16A4();
	}
	if(PlayerWeaponSocketsName[M870].Contains(WeaponActor->GetStructure().Model))
	{
		AttachM870();
	}
	if(PlayerWeaponSocketsName[HK416].Contains(WeaponActor->GetStructure().Model))
	{
		AttachHK416();
	}
}

void AMainPlayer::CreateWeaponAttach(const FString& ModelName)
{
	if(PlayerWeaponSocketsName[AWM].Contains(ModelName))
	{
		AttachAWM();
	}
	if(PlayerWeaponSocketsName[AK47].Contains(ModelName))
	{
		AttachAK47();
	}
	if(PlayerWeaponSocketsName[M16A4].Contains(ModelName))
	{
		AttachM16A4();
	}
	if(PlayerWeaponSocketsName[M870].Contains(ModelName))
	{
		AttachM870();
	}
	if(PlayerWeaponSocketsName[HK416].Contains(ModelName))
	{
		AttachHK416();
	}
}

void AMainPlayer::AttachAWM()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = GetWorld()->SpawnActor<AWeaponClass>(AwmClass, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	if(EquippedWeapon)
	{
		EquippedWeapon->SetActorEnableCollision(false);
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), *PlayerWeaponSocketsName[AWM]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AWM"));
	}
}

void AMainPlayer::AttachAK47()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = GetWorld()->SpawnActor<AWeaponClass>(Ak47Class, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	if(EquippedWeapon)
	{
		EquippedWeapon->SetActorEnableCollision(false);
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), *PlayerWeaponSocketsName[AK47]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AK47"));
	}
}

void AMainPlayer::AttachM16A4()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = GetWorld()->SpawnActor<AWeaponClass>(M16A4Class, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	if(EquippedWeapon)
	{
		EquippedWeapon->SetActorEnableCollision(false);
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), *PlayerWeaponSocketsName[M16A4]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AK47"));
	}
}

void AMainPlayer::AttachM870()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = GetWorld()->SpawnActor<AWeaponClass>(M870Class, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	if(EquippedWeapon)
	{
		EquippedWeapon->SetActorEnableCollision(false);
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), *PlayerWeaponSocketsName[M870]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AK47"));
	}
}

void AMainPlayer::AttachHK416()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = GetWorld()->SpawnActor<AWeaponClass>(Hk416Class, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	if(EquippedWeapon)
	{
		EquippedWeapon->SetActorEnableCollision(false);
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), *PlayerWeaponSocketsName[HK416]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AK47"));
	}
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
