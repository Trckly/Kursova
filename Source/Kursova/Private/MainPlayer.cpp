// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "../ServerLogic/UI/ServerWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kursova/MainUI/MainMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kursova/AI/UEnemyInterface.h"
#include "Kursova/Command/NegativeRotationCommand.h"
#include "Kursova/Command/PositiveRotationCommand.h"
#include "Kursova/Core//CustomPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kursova/UMG/CrosshairWidget.h"
#include "Kursova/HUD/PlayerHUD.h"
#include "Kursova/Items/AbstractItem.h"
#include "Kursova/Items/CoolItem.h"
#include "Kursova/Items/Chain/ArmorHandler.h"
#include "Kursova/Items/Chain/HealthHandler.h"
#include "Kursova/ObjectDecorator/Cube.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetupAttachment(GetMesh(), "head");

	IsInGodMode = false;
	BehaviorSet = {true, true, true};
	Health = 100.f;
	bReplicates = true;
	MouseSensivity = 50.f;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	///
	/// Server Logic
	///
	CreateServerWidget();

	CreateMainMenuWidget();

	CreateHUDWidget();
	
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

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::OnBeginOverlap);
}

void AMainPlayer::CreateServerWidget()
{
	APlayerController* PlayerController = GetController<APlayerController>();
	
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
		MainMenuWidget = CreateWidget<UMainMenuWidget>(PController, MainMenuWidgetClass);

		if(MainMenuWidget)
		{
			MainMenuWidget->SetUserInfo.BindDynamic(this, &AMainPlayer::SetNameAndCity);
			MainMenuWidget->SetDifficultyMode.BindDynamic(this, &AMainPlayer::SetDifficultyMode);
			MainMenuWidget->AddToViewport();
			
			PController->SetShowMouseCursor(true);
			PController->SetInputMode(FInputModeGameAndUI());
		}
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayer::MoveRight);
	
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AMainPlayer::Interact);
	PlayerInputComponent->BindAction(TEXT("Escape"), IE_Pressed, this, &AMainPlayer::ContinueGameplay);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AMainPlayer::Shoot);
	PlayerInputComponent->BindAction(TEXT("Heal"), IE_Pressed, this, &AMainPlayer::SetHealth);
	PlayerInputComponent->BindAction(TEXT("SetArmor"), IE_Pressed, this, &AMainPlayer::SetArmor);
	PlayerInputComponent->BindAction(TEXT("HealSetArmor"), IE_Pressed, this, &AMainPlayer::SetHealthArmor);

	///
	/// Server Logic
	///
	PlayerInputComponent->BindAction("OpenServerMenu", IE_Pressed, this, &AMainPlayer::AddServerWidgetToViewPort);
}

void AMainPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AMainPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if(Cast<ACoolItem>(OtherActor)) return;

	AAbstractItem* Item = Cast<AAbstractItem>(OtherActor);

	if(Item)
	{
		PickUpItems.Add(Item->CreatePocketCopy());
		Item->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Item was picked up"));
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

// void AMainPlayer::LookRight(float Value)
// {
// 	if(Value != 0)
// 	{
// 		float SensivityScalar = MouseSensivity * GetWorld()->GetDeltaSeconds();
// 		AddControllerYawInput(Value * SensivityScalar);
// 	}
// }

void AMainPlayer::Jump()
{
	if(BehaviorSet.CanJump)
	{
		Super::Jump();
		AnimNotify_Jump.Broadcast();
	}
}

UCameraComponent* AMainPlayer::GetCameraComponent()
{
	return CameraComponent;
}

void AMainPlayer::SetHealth()
{
	UHealthHandler* HealthHandler = NewObject<UHealthHandler>(this, UHealthHandler::StaticClass(), TEXT("HealthHandler"));
	HealthHandler->AddHealth.BindDynamic(this, &AMainPlayer::AMainPlayer::AddHealth);
	ActivateBuffs(HealthHandler);
}

void AMainPlayer::SetArmor()
{
	UArmorHandler* ArmorHandler = NewObject<UArmorHandler>(this, UArmorHandler::StaticClass(), TEXT("ArmorHandler"));
	ArmorHandler->AddArmor.BindDynamic(this, &AMainPlayer::AddArmor);
	ActivateBuffs(ArmorHandler);
}

void AMainPlayer::SetHealthArmor()
{
	UHealthHandler* HealthHandler = NewObject<UHealthHandler>(this, UHealthHandler::StaticClass(), TEXT("HealthHandler"));
	HealthHandler->AddHealth.BindDynamic(this, &AMainPlayer::AMainPlayer::AddHealth);
	UArmorHandler* ArmorHandler = NewObject<UArmorHandler>(this, UArmorHandler::StaticClass(), TEXT("ArmorHandler"));
	ArmorHandler->AddArmor.BindDynamic(this, &AMainPlayer::AddArmor);
	
	HealthHandler->SetNext(ArmorHandler);

	ActivateBuffs(HealthHandler);
}

void AMainPlayer::ActivateBuffs(const TScriptInterface<IHandler>& Handler)
{
	if(PickUpItems.Num() == 0)
		return;

	TArray<AAbstractItem*> ToSave;
	while(PickUpItems.Num() != 0)
	{
		AAbstractItem* Item = PickUpItems.Pop();
		
		if(Handler->Handle(Item))
		{
			Item->Destroy();
		}else
		{
			ToSave.Add(Item);
		}
	}

	PickUpItems.Append(ToSave);
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
		else if(Cast<ACube>(ReturnedActor))
		{
			ProcessHitCube(ReturnedActor);
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
	
	CrosshairWidget->RemoveFromParent();
	if(const auto CreatedWeaponMenu = CreateWidget<UWeaponMenuWidget>(GetWorld(), WeaponMenuWidgetClass))
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

void AMainPlayer::ProcessHitCube(AActor* HitActor)
{
	APlayerController* PController = Cast<APlayerController>(GetOwner());
	if(PController)
	{
		UCommandWidget* CommandWidget = CreateWidget<UCommandWidget>(PController, CommandWidgetClass);

		CommandWidget->PositiveRotationDelegate.BindDynamic(this, &AMainPlayer::RotatePositive);
		CommandWidget->NegativeRotationDelegate.BindDynamic(this, &AMainPlayer::RotateNegative);
		CommandWidget->WidgetClosed.BindDynamic(this, &AMainPlayer::OnCommandWidgetClosed);

		CommandWidget->PassActor(HitActor);
		
		CommandWidget->AddToViewport();
		
		PController->SetShowMouseCursor(true);
		PController->SetInputMode(FInputModeUIOnly());
	}
}

void AMainPlayer::RotatePositive(AActor* Actor)
{
	UPositiveRotationCommand* PositiveRotationCommand = NewObject<UPositiveRotationCommand>();
	if(PositiveRotationCommand)
	{
		PositiveRotationCommand->Execute(Actor);
	}
}

void AMainPlayer::RotateNegative(AActor* Actor)
{
	UNegativeRotationCommand* NegativeRotationCommand = NewObject<UNegativeRotationCommand>();
	if(NegativeRotationCommand)
	{
		NegativeRotationCommand->Execute(Actor);
	}
}

void AMainPlayer::OnCommandWidgetClosed()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if(PlayerController)
	{
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

TArray<AWeaponClass*> AMainPlayer::GetAllPickedWeapons()
{
	return PickedWeapons;
}

void AMainPlayer::Shoot()
{
	
	if(!BehaviorSet.CanShoot)
	{
		return;
	}
	FVector StartTrace = CameraComponent->GetComponentLocation();
	FVector EndTrace = CameraComponent->GetComponentLocation() + CameraComponent->GetComponentRotation().Vector() * 10000.f;
	
	Server_Shoot(StartTrace, EndTrace);
	
	
}

void AMainPlayer::Server_Shoot_Implementation(FVector StartTrace, FVector EndTrace)
{
	Multicast_Shoot(StartTrace, EndTrace);
}

void AMainPlayer::Multicast_Shoot_Implementation(FVector StartTrace, FVector EndTrace)
{

	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTrace, EndTrace,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, {this},
		EDrawDebugTrace::ForDuration, HitResult, true, FColor::Red, FColor::Green,
		2.f);
	
	IEnemyInterface* Enemy = Cast<IEnemyInterface>(HitResult.GetActor());
	if(Enemy)
	{
		Enemy->GetDamage(20);
		UE_LOG(LogTemp, Warning, TEXT("Enemy was hit"));
	}

}

void AMainPlayer::GetDamage(int Damage)
{
	if(!IsInGodMode)
	{
		if(Health <= 0.f)
		{
			return;
		}
		Health -= Damage;
		Health = FMath::Clamp(Health, 0.f, 100.f);
		
		if(Health <= 50 && HealthState > 0)
		{
			HealthState = -1;
			Notify();
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Player got %d damage"), Damage);

		if(PlayerHUDWidget != nullptr)
		{
			PlayerHUDWidget->SetHealth(Health);
		}
	}
}

void AMainPlayer::AddHealth(float HealthPoints)
{
	if(Health < 100)
	{
		Health += HealthPoints;

		Health = FMath::Clamp(Health, 0.f, 100.f);

		if(Health > 50 && HealthState < 0)
		{
			HealthState = 1;
			Notify();
		}

		if(PlayerHUDWidget != nullptr)
		{
			PlayerHUDWidget->SetHealth(Health);
		}
	}
}

void AMainPlayer::AddArmor(float ArmorPoints)
{
	
	if(Armor < 100)
	{
		Armor += ArmorPoints;

		Armor = FMath::Clamp(Armor, 0.f, 100.f);

		if(PlayerHUDWidget != nullptr)
		{
			PlayerHUDWidget->SetArmor(Armor);
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

void AMainPlayer::CreateSession(FString Name, bool Privacy, FString Password, int InputNumberOfPlayers, FString Map, FString Difficulty)
{
	if(ServerWidget && ServerWidget->IsInViewport())
	{
		ServerWidget->RemoveFromParent();
	}
	GetController<APlayerController>()->SetShowMouseCursor(false);
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());

	CreateS(InputNumberOfPlayers, Name, Privacy, Password, Map, Difficulty);
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
		ServerWidget->RemoveFromParent();
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
		MainMenuWidget->RemoveFromParent();

		if(PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		
			PController->SetShowMouseCursor(false);
			PController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AMainPlayer::SetDifficultyMode(const FString& Difficulty)
{
	TScriptInterface<IModeFactory> Factory = SetDifficultyModeDelegate.Execute(Difficulty);
	if(!Factory)
	{
		UE_LOG(LogActor, Error, TEXT("Delegate SetDifficultyMode is not bound"));
	}
	else
	{
		IWeaponInterface* Weapon = Factory->CreateWeapon();
		if(!Weapon)
		{
			UE_LOG(LogActor, Error, TEXT("Factory failed to create weapon"));
		}
		else
		{
			if(EquippedDifficultyWeapon)
			{
				EquippedDifficultyWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
				EquippedDifficultyWeapon->Destroy();
			}

			EquippedDifficultyWeapon = Weapon;
			EquippedDifficultyWeapon->SetActorEnableCollision(false);
			EquippedDifficultyWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), *PlayerWeaponSocketsName[AK47]);
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

void AMainPlayer::Attach(IEnemyObserver* EnemyInterface)
{
	EnemyObservers.Add(EnemyInterface);
	UE_LOG(LogTemp, Warning, TEXT("Enemy was attached!"));
}

void AMainPlayer::Detach(IEnemyObserver* EnemyInterface)
{
	EnemyObservers.Remove(EnemyInterface);
}

void AMainPlayer::Notify()
{
	for(auto Observer : EnemyObservers)
	{
		if(Observer == nullptr)
		{
			EnemyObservers.Remove(Observer);
			continue;
		}
		Observer->Update(Health);
	}
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
