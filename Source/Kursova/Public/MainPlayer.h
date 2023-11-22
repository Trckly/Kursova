// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeaponClass.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "../ServerLogic/SessionSubsystem.h"
#include "Kursova/UMG/WeaponMenuWidget.h"
#include "MainPlayer.generated.h"

class UMainMenuWidget;
class UServerWidget;
class AMainPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterJoinSession, FBlueprintSessionResult, SessionResult, const FString&, Password);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSetStats, AMainPlayer*, Self);

USTRUCT()
struct FBehaviorSet
{
	GENERATED_BODY()
	
	bool CanMove;
	bool CanJump;
	bool CanShoot;
};

/**
 Player character class
 */

UCLASS()
class KURSOVA_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	AMainPlayer();

	// Default destructor override
	virtual ~AMainPlayer() override = default;

	// Player's camera component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	// Defines how far can player reach to interact with objects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InteractDistance = 200.f;

	// Defines whether game can be continued or not
	UPROPERTY(BlueprintReadOnly)
	bool bContinuable = false;
		
	// Positions of player before teleportation
	FVector PreviousActorLocation;
	FRotator PreviousActorRotation;

	// Positions to teleport player in order to use menu
	FVector WeaponChooseLocation = FVector(200.f, -810.f, 152.f);
	FRotator WeaponChooseRotation = FRotator(0.f, -90.f, 0.f);

	// Defines whether to show crosshair or not
	UPROPERTY(BlueprintReadOnly)
	bool bShowCrosshair = true;

	// Stores pointers to all picked up weapons
	UPROPERTY(BlueprintReadOnly)
	TArray<AWeaponClass*> PickedWeapons;

	///
	///	Weapon blueprint classes
	///
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AWeaponClass> AwmClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AWeaponClass> M16A4Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AWeaponClass> Ak47Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AWeaponClass> M870Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AWeaponClass> Hk416Class;

	///
	/// Player SK_Mesh weapon sockets enumaeration
	/// 
	enum EPlayerWeaponSockets
	{
		AWM,
		AK47,
		M16A4,
		M870,
		HK416
	};

	// Names of mesh sockets
	TArray<FString> PlayerWeaponSocketsName;
	
	// Equipped weapon
	UPROPERTY()
	AWeaponClass* EquippedWeapon;
	
	///
	/// Andrii Kursova
	///
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FString SPlayerName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FString SCity;

	UPROPERTY(Replicated)
	bool IsInGodMode;

	UPROPERTY(Replicated)
	FBehaviorSet BehaviorSet;

    UPROPERTY(Replicated)
	float Health;

	int PlayerIndex;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponMenuWidget> WeaponMenuWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY()
	UWeaponMenuWidget* WeaponMenuWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	
	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;
	
	/// 
	/// Common
	///

	// Basic move forward function
	void MoveForward(float Scale);

	// Basic move right function
	void MoveRight(float Scale);

	// Delegate function. Defines which object was hit
	// by trace with interactive channel active 
	UFUNCTION(BlueprintCallable)
	void Interact();

	// Exits from weapon menu
	UFUNCTION(BlueprintCallable)
	void ContinueGameplay();

	// Prepares and opens weapon menu
	void ProcessHitRack();

	// Collecting hit weapon
	void ProcessHitWeapon(AWeaponClass* WeaponActor);

	// Return all player's picked weapons
	TArray<AWeaponClass*> GetAllPickedWeapons();

	// Delegate function. Defines which object was hit
	// by trace with enemy channel active
	UFUNCTION()
	void Shoot();

	// Setting weapon into player's hands
	UFUNCTION()
	void CreateWeaponAttach(AWeaponClass* WeaponActor);
	void CreateWeaponAttach(const FString& ModelName);

	///
	/// Weapon attachment functions
	/// 
	void AttachAWM();
	void AttachAK47();
	void AttachM16A4();
	void AttachM870();
	void AttachHK416();
	
	///
	/// ServerLogic
	///
	UPROPERTY(EditAnywhere)
	TSubclassOf<UServerWidget> ServerWidgetClass;

	UPROPERTY(EditAnywhere)
	UServerWidget* ServerWidget;	

	UPROPERTY()
	FSetStats SetStats;
	
	UFUNCTION()
	void CreateSession(FString Name, bool IsPrivate, FString Password, int NumberOfPlayers);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateS(int NumberOfPlayers,const FString& SessionName, bool IsPrivate, const FString& Password);

	UFUNCTION(BlueprintImplementableEvent)
	void FindS();

	UPROPERTY(BlueprintAssignable)
	FOnCharacterJoinSession OnCharacterJoinSession;

	UFUNCTION()
	void AddServerWidgetToViewPort();

	UFUNCTION()
	void ConnectToService();

	UFUNCTION()
	void JoinSession(FBlueprintSessionResult SessionResult, const FString& Password);

	UFUNCTION(BlueprintCallable)
	void SetSessionsToWidget(TArray<FBlueprintSessionResult> BlueprintSessionResults);
	
	///
	/// Andrii Kursova
	///
	void SetName(FString Name);
	
	FString GetPlayerName() const;

	void SetCity(FString City);
	
	FString GetCity() const;

	FBehaviorSet GetBehaviorSet() const;

	bool GetGodModeState() const;

	void SetGodModeState(bool HasGodMode);

	float GetHealth() const;

	int GetPlayerIndex() const;

	void SetPlayerIndex(int Index);

	UFUNCTION(Server, Reliable)
	void ServerSetNameAndCity(FString const& Name, FString const& City);

	UFUNCTION()
	void SetNameAndCity(FString const& Name, FString const& City);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetNameAndCity(FString const& Name, FString const& City);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetBehavior(bool CanMove, bool CanJump, bool CanFire);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetGodMode(bool IsGodModeSet);

	void WriteWeaponsFromFile();
};
