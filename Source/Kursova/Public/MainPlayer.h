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

	UPROPERTY(BlueprintReadOnly)
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

	UFUNCTION(Server, Reliable)
	void Server_DealDamage(int Damage);

	UFUNCTION(Client, Reliable)
	void Client_DealDamage(int Damage);

	UFUNCTION()
	void DealDamage(int Damage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DealDamage(int Damage);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateWeaponAttach();

	// UFUNCTION()
	// void AdjustCameraRotation();
	//
	// UFUNCTION(Server, Reliable)
	// void Server_UpdateCameraRotation(float CamRotation);

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