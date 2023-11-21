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
#include "Kursova/HUD/PlayerHUD.h"

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
	
	std::cout << *this;
}

AMainPlayer::AMainPlayer(AMainPlayer& OtherPlayer)
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName(*OtherPlayer.CameraComponent->GetName()));
	CameraComponent->SetRelativeLocation(OtherPlayer.CameraComponent->GetUpVector());
	CameraComponent->bUsePawnControlRotation = OtherPlayer.CameraComponent->bUsePawnControlRotation;
	CameraComponent->SetupAttachment(GetMesh(), "head");
	
	IsInGodMode = OtherPlayer.IsInGodMode;
	BehaviorSet = OtherPlayer.BehaviorSet;
	Health = OtherPlayer.Health;
	bReplicates = OtherPlayer.bReplicates;
}

AMainPlayer::AMainPlayer(bool GodMode, FBehaviorSet Behavior, float HP)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetRelativeLocation(CameraComponent->GetUpVector() * 90.f);
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetupAttachment(GetMesh(), "head");

	IsInGodMode = GodMode;
	BehaviorSet = Behavior;
	Health = HP;
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
		if(PlayerHUDWidgetClass)
		{
			PlayerHUDWidget = CreateWidget<UPlayerHUD>(PController, PlayerHUDWidgetClass);

			if(PlayerHUDWidget)
			{
				PlayerHUDWidget->SetHealth(Health);
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

	CreateWeaponAttach();
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMainPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AMainPlayer::Interact);
	PlayerInputComponent->BindAction(TEXT("Escape"), IE_Pressed, this, &AMainPlayer::ContinueGameplay);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AMainPlayer::Server_Shoot);

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
	if(Scale != 0.f && BehaviorSet.CanMove)
	{
		AddMovementInput(GetActorForwardVector(), Scale);
	}
}

void AMainPlayer::MoveRight(float Scale)
{
	if(Scale != 0.f && BehaviorSet.CanMove)
	{
		AddMovementInput(GetActorRightVector(), Scale);
	}
}

void AMainPlayer::Jump()
{
	if(BehaviorSet.CanJump)
	{
		Super::Jump();
	}
}

void AMainPlayer::Server_Turn_Implementation(float Rate)
{
	Multicast_Turn(Rate);
}

void AMainPlayer::Multicast_Turn_Implementation(float Rate)
{
	TurnRate = Rate;
	AddControllerYawInput(Rate * 45.f * GetWorld()->GetDeltaSeconds());
}

void AMainPlayer::Server_LookUp_Implementation(float Rate)
{
	Multicast_LookUp(Rate);
}

void AMainPlayer::Multicast_LookUp_Implementation(float Rate)
{
	AddControllerPitchInput(Rate * 45.f * GetWorld()->GetDeltaSeconds());
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

void AMainPlayer::Server_Shoot_Implementation()
{
	Multicast_Shoot();
}

void AMainPlayer::Multicast_Shoot_Implementation()
{
	if(!BehaviorSet.CanShoot)
	{
		return;
	}
	
	FHitResult HitResult;
	FVector StartTrace = CameraComponent->GetComponentLocation();
	FVector EndTrace = CameraComponent->GetComponentLocation() + GetActorForwardVector() * 10000.f;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTrace, EndTrace,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, {this},
		EDrawDebugTrace::ForDuration, HitResult, true, FColor::Red, FColor::Green,
		2.f);
	
	AMainPlayer* Enemy = Cast<AMainPlayer>(HitResult.GetActor());
	if(Enemy)
	{
		
		Enemy->DealDamage(30.f);
		//Enemy->DealDamage(30.f);
	}

}

void AMainPlayer::Server_DealDamage_Implementation(int Damage)
{
	Multicast_DealDamage(Damage);
}

void AMainPlayer::Client_DealDamage_Implementation(int Damage)
{
	
}

void AMainPlayer::DealDamage(int Damage)
{
	if(!IsInGodMode)
	{
		Health -= Damage;

		if(PlayerHUDWidget != nullptr)
		{
			PlayerHUDWidget->SetHealth(Health);
		}
	}
}

void AMainPlayer::Multicast_DealDamage_Implementation(int Damage)
{
	if(!IsInGodMode)
	{
		Health -= Damage;

		if(PlayerHUDWidget != nullptr)
		{
			PlayerHUDWidget->SetHealth(Health);
		}
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
		PlayerHUDWidget->AddToViewport();
		
		PController->SetShowMouseCursor(false);
		PController->SetInputMode(FInputModeGameOnly());
	}
}

void AMainPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainPlayer, SPlayerName);
	DOREPLIFETIME(AMainPlayer, SCity);
	DOREPLIFETIME(AMainPlayer, IsInGodMode);
	DOREPLIFETIME(AMainPlayer, TurnRate);
	DOREPLIFETIME(AMainPlayer, LookUpRate);
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



std::ostream& operator<<(std::ostream& OStream, const AMainPlayer& MPlayer)
{
	FString Output;
	Output += MPlayer.SPlayerName + ' '
			+ MPlayer.SCity + ' '
			+ (MPlayer.BehaviorSet.CanMove ? FString("true") : FString("false")) + ' '
			+ (MPlayer.BehaviorSet.CanJump ? FString("true") : FString("false")) + ' '
			+ (MPlayer.BehaviorSet.CanShoot ? FString("true") : FString("false")) + ' '
			+ (MPlayer.IsInGodMode ? FString("true") : FString("false"));
	OStream << *Output;
	return OStream;
}

std::istream& operator>>(std::istream& IStream, AMainPlayer& MPlayer)
{
	std::string LogString;
    
	// Read a line from the file

	if (std::getline(IStream, LogString))
	{
		std::istringstream IsRealString(LogString);
		TArray<std::string> ArrOfStrings;
		ArrOfStrings.SetNum(6);
		if (!(IsRealString >> ArrOfStrings[0] >> ArrOfStrings[1] >> ArrOfStrings[2] >> ArrOfStrings[3] >> ArrOfStrings[4] >> ArrOfStrings[5]))
		{
			//throw ExceptionWeaponOutput("Corrupted file! Missing data!");
		}
		
		// Weapon.WeaponUnit.Model = ArrOfStrings[0].c_str();
		MPlayer.SPlayerName = FString(ArrOfStrings[0].c_str());
		MPlayer.SCity = FString(ArrOfStrings[1].c_str());
		MPlayer.BehaviorSet.CanMove = ArrOfStrings[2] == "true" ? true : false;
		MPlayer.BehaviorSet.CanJump = ArrOfStrings[3] == "true" ? true : false;
		MPlayer.BehaviorSet.CanShoot = ArrOfStrings[4] == "true" ? true : false;
		MPlayer.IsInGodMode = ArrOfStrings[5] == "true" ? true : false;
	}
	else
	{
		//throw ExceptionWeaponOutput("Cannot read data from file!");
	}

	return IStream;
}