// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeaponClass.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "../ServerLogic/SessionSubsystem.h"
#include "GenericTeamAgentInterface.h"
#include "Kursova/Command/Widgets/CommandWidget.h"
#include "Kursova/UMG/WeaponMenuWidget.h"
#include "Kursova/DifficultyFactories/ModeFactory.h"
#include "Kursova/EnemyObserver/PlayerNotifier.h"
#include "Kursova/ObjectDecorator/Cube.h"
#include "MainPlayer.generated.h"

class IHandler;
class AAbstractItem;
class UPlayerHUD;
class UMainMenuWidget;
class UServerWidget;
class AMainPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimNotify_Jump);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterJoinSession, FBlueprintSessionResult, SessionResult, const FString&, Password);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSetStats, AMainPlayer*, Self);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(TScriptInterface<IModeFactory>, FDifficultyModeSet, const FString&, Difficulty);

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
class KURSOVA_API AMainPlayer : public ACharacter, public IGenericTeamAgentInterface, public IPlayerNotifier
{
	GENERATED_BODY()

	FGenericTeamId TeamId;

protected:
	// Sets default values for this character's properties
	AMainPlayer();

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
	
	// Pointer to all weapon instances picked by player
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<AWeaponClass*> PickedWeapons;

	UPROPERTY(Replicated)
	float TurnRate;

	UPROPERTY(Replicated)
	float LookUpRate;

	UPROPERTY(EditDefaultsOnly)
	float MouseSensivity;
	
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

	IWeaponInterface* EquippedDifficultyWeapon;
	
	///
	/// Andrii Kursova
	///
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FString SPlayerName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FString SCity;

	UPROPERTY(Replicated)
	bool IsInGodMode;

	UPROPERTY()
	FBehaviorSet BehaviorSet;

    UPROPERTY()
	float Health;

	int HealthState = 1;

    UPROPERTY()
	float Armor;

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

	UFUNCTION()
	void CreateServerWidget();

	UFUNCTION()
	void CreateHUDWidget();

	UFUNCTION()
	void CreateMainMenuWidget();

	UFUNCTION()
	void SetDifficultyMode(const FString& Difficulty);
	
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

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	TArray<AAbstractItem*> PickUpItems;

	// Basic move forward function
	void MoveForward(float Scale);

	// Basic move right function
	void MoveRight(float Scale);
	
	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetCameraComponent();

	UFUNCTION()
	void SetHealth();

	UFUNCTION()
	void SetArmor();

	UFUNCTION()
	void SetHealthArmor();

	UFUNCTION()
	void ActivateBuffs(const TScriptInterface<IHandler>& Handler);
	
	UFUNCTION(Server, Reliable)
	void Server_Turn(float Rate);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Turn(float Rate);

	UFUNCTION(Server, Reliable)
	void Server_LookUp(float Rate);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LookUp(float Rate);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UCommandWidget> CommandWidgetClass;

	UFUNCTION()
	void RotatePositive(AActor* Actor);

	UFUNCTION()
	void RotateNegative(AActor* Actor);

	UFUNCTION()
	void OnCommandWidgetClosed();
	
	void ProcessHitCube(AActor* HitActor);

	// Return all player's picked weapons
	TArray<AWeaponClass*> GetAllPickedWeapons();

	UFUNCTION()
	void Shoot();
	
	UFUNCTION(Server, Reliable)
	void Server_Shoot(FVector StartTrace, FVector EndTrace);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Shoot(FVector StartTrace, FVector EndTrace);
	
	UFUNCTION()
	void GetDamage(int Damage);

	UFUNCTION()
	void AddHealth(float HealthPoints);

	UFUNCTION()
	void AddArmor(float ArmorPoints);

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUD> PlayerHUDWidgetClass;

	UPROPERTY()
	UPlayerHUD* PlayerHUDWidget;
	
	///
	/// ServerLogic
	///
	UPROPERTY(EditAnywhere)
	TSubclassOf<UServerWidget> ServerWidgetClass;

	UPROPERTY()
	UServerWidget* ServerWidget;

	UPROPERTY()
	FSetStats SetStats;

	FDifficultyModeSet SetDifficultyModeDelegate;
	
	UFUNCTION()
	void CreateSession(FString Name, bool IsPrivate, FString Password, int NumberOfPlayers, FString Map, FString Difficulty);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateS(int NumberOfPlayers,const FString& SessionName, bool IsPrivate, const FString& Password, const FString& Map, const FString& Difficulty);

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

	//Team
	virtual FGenericTeamId GetGenericTeamId() const override{return TeamId;}

	UPROPERTY(BlueprintAssignable)
	FAnimNotify_Jump AnimNotify_Jump;

	// Observer Logic
	TArray<IEnemyObserver*> EnemyObservers;
	
	virtual void Attach(IEnemyObserver* EnemyInterface) override;
	virtual void Detach(IEnemyObserver* EnemyInterface) override;
	virtual void Notify() override;
};
