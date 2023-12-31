// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "../ServerLogic/UI/ServerWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kursova/MainUI/MainMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kursova/Core/CustomPlayerController.h"
#include "Kursova/Exceptions/ExceptionPlayerLog.h"
#include "Kursova/Exceptions/ExceptionPlayerWidget.h"
#include "Kursova/Exceptions/ExceptionWeaponOutput.h"
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
	try
	{
		CreateServerWidget();

		CreateMainMenuWidget();

		CreateHUDWidget();
	}
	catch(const ExceptionPlayerWidget& Except)
	{
		FString ExceptStr(Except.what());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ExceptStr);
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

void AMainPlayer::CreateServerWidget()
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if(!ServerWidgetClass)
	{
		throw ExceptionPlayerWidget("Unable to create Server Widget");
	}
	
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

void AMainPlayer::CreateHUDWidget()
{
	ACustomPlayerController* PController = Cast<ACustomPlayerController>(GetOwner());
	
	if(PController)
	{
		if(!PlayerHUDWidgetClass)
		{
			throw ExceptionPlayerWidget("Unable to create HUD");
		}
		
		PlayerHUDWidget = CreateWidget<UPlayerHUD>(PController, PlayerHUDWidgetClass);

		if(PlayerHUDWidget)
		{
			PlayerHUDWidget->SetHealth(Health);
		}
	}
}

void AMainPlayer::CreateMainMenuWidget()
{
	ACustomPlayerController* PController = Cast<ACustomPlayerController>(GetOwner());
	if(PController)
	{
		if(!MainMenuWidgetClass)
		{
			throw ExceptionPlayerWidget("Unable to create Main Menu Widget");
		}
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

void AMainPlayer::WriteLog()
{
	std::ofstream LogFile;
	LogFile.open("D:\\Kursova\\Kursova\\Content\\StuffLog.txt", std::ios::trunc);

	LogFile << *this;

	LogFile.close();
}

void AMainPlayer::ReadLog()
{
	std::ifstream LogFile;
	LogFile.open("D:\\Kursova\\Kursova\\Content\\StuffLog.txt", std::ios::in);

	try
	{
		LogFile >> *this;
	}
	catch(const ExceptionPlayerLog& Except)
	{
		FString ExceptStr(Except.what());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ExceptStr);
	}

	LogFile.close();
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
	FVector EndTrace = CameraComponent->GetComponentLocation() + GetControlRotation().Vector() * 10000.f;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTrace, EndTrace,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, {this},
		EDrawDebugTrace::ForDuration, HitResult, true, FColor::Red, FColor::Green,
		2.f);
	
	AMainPlayer* Enemy = Cast<AMainPlayer>(HitResult.GetActor());
	if(Enemy)
	{
		Enemy->DealDamage(20.f);
	}

}

void AMainPlayer::DealDamage(int Damage)
{
	if(!IsInGodMode)
	{
		if(Health <= 0.f)
		{
			return;
		}
		Health -= Damage;

		if(PlayerHUDWidget != nullptr)
		{
			PlayerHUDWidget->SetHealth(Health);
		}
	}
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

		if(PlayerHUDWidget)
		{
			WriteLog();
			ReadLog();
			PlayerHUDWidget->AddToViewport();
		
			PController->SetShowMouseCursor(false);
			PController->SetInputMode(FInputModeGameOnly());
		}
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
	DOREPLIFETIME(AMainPlayer, PickedWeapons);
}

void AMainPlayer::WriteWeaponsFromFile()
{
	std::ifstream Fin("WeaponDataConfig.txt");
	std::string Line;
	while (std::getline(Fin, Line))
	{
		FString FLine(UTF8_TO_TCHAR(Line.c_str()));
		TArray<FString> Words;
		FLine.ParseIntoArray(Words, TEXT(" "));
		if(Words.Num() < 9)
		{
			throw ExceptionWeaponOutput("One of rows in file is corrupted!");
		}
		FWeaponUnit NewUnit;
		NewUnit.Model = Words[0];
		NewUnit.MainType = Words[1];
		NewUnit.Subtype = Words[2];
		NewUnit.Capacity = FCString::Atoi(*Words[3]);
		NewUnit.Manufacturer = Words[4];
		NewUnit.Caliber = FCString::Atof(*Words[5]);
		NewUnit.Length = FCString::Atoi(*Words[6]);
		NewUnit.Weight = FCString::Atoi(*Words[7]);
		NewUnit.Price = FCString::Atoi(*Words[8]);

		// AWeaponClass* NewWeapon = new AWeaponClass;
		// NewWeapon->InitWithStruct(NewUnit);
		// PickedWeapons.Push(NewWeapon);
	}
	Fin.close();
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
			throw ExceptionPlayerLog("Corrupted file! Missing data!");
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
		throw ExceptionPlayerLog("Cannot read data from file!");
	}

	return IStream;
}