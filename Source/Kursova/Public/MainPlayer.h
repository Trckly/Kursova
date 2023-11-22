// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeaponClass.h"
#include "WeaponMenuWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "../ServerLogic/SessionSubsystem.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "MainPlayer.generated.h"

class UPlayerHUD;
class UMainMenuWidget;
class UServerWidget;
class AMainPlayer;

DECLARE_DELEGATE(FRackDelegate);
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

UCLASS()
class KURSOVA_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	AMainPlayer();

	AMainPlayer(AMainPlayer& OtherPlayer);
	
	AMainPlayer(bool GodMode, FBehaviorSet Behavior, float HP);
	
	virtual ~AMainPlayer() override = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InteractDistance = 200.f;

	UPROPERTY(BlueprintReadOnly)
	bool bContinuable = false;

	TSubclassOf<AActor*> RackClass;

	FVector PreviousActorLocation;
	FRotator PreviousActorRotation;

	FVector WeaponChooseLocation = FVector(200.f, -810.f, 152.f);
	FRotator WeaponChooseRotation = FRotator(0.f, -90.f, 0.f);
	
	UPROPERTY(BlueprintReadOnly)
	bool bShowCrosshair = true;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<AWeaponClass*> PickedWeapons;

	UPROPERTY(Replicated)
	float TurnRate;

	UPROPERTY(Replicated)
	float LookUpRate;
	
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

	UFUNCTION()
	void CreateServerWidget();

	UFUNCTION()
	void CreateHUDWidget();

	UFUNCTION()
	void CreateMainMenuWidget();

	UFUNCTION()
	void WriteLog();

	UFUNCTION()
	void ReadLog();
	
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
	void MoveForward(float Scale);

	void MoveRight(float Scale);

	virtual void Jump() override;

	UFUNCTION(Server, Reliable)
	void Server_Turn(float Rate);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Turn(float Rate);

	UFUNCTION(Server, Reliable)
	void Server_LookUp(float Rate);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LookUp(float Rate);

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void ContinueGameplay();

	void ProcessHitRack();

	void ProcessHitWeapon(AWeaponClass* WeaponActor);

	TArray<AWeaponClass*> GetAllPickedWeapons();

	UFUNCTION(Server, Reliable)
	void Server_Shoot();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Shoot();

	UFUNCTION()
	void DealDamage(int Damage);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateWeaponAttach();

	FRackDelegate RackDelegate;
	
	
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

	friend std::ostream& operator<<(std::ostream& OStream, const AMainPlayer& MPlayer );
	friend std::istream& operator>>(std::istream& IStream, AMainPlayer& MPlayer );
};