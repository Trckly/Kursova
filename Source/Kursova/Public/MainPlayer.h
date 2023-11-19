// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeaponClass.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "../ServerLogic/SessionSubsystem.h"
#include "MainPlayer.generated.h"

class UServerWidget;

DECLARE_DELEGATE(FRackDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterJoinSession, FBlueprintSessionResult, SessionResult, const FString&, Password);

typedef struct FBehaviorSet
{
	bool CanMove;
	bool CanJump;
	bool CanShoot;
} FBehaviorSet;

UCLASS()
class KURSOVA_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	AMainPlayer();

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

	///
	/// Andrii Kursova
	///
	FString SPlayerName;

	FString SCity;

	bool IsInGodMode;
	
	FBehaviorSet BehaviorSet;

	float Health;

	int PlayerIndex;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Scale);

	void MoveRight(float Scale);

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void ContinueGameplay();

	void ProcessHitRack();

	void ProcessHitWeapon(AWeaponClass* WeaponActor);

	TArray<AWeaponClass*> GetAllPickedWeapons();

	FRackDelegate RackDelegate;
	
	///
	/// ServerLogic
	///

	UPROPERTY(EditAnywhere)
	TSubclassOf<UServerWidget> ServerWidgetClass;

	UPROPERTY(EditAnywhere)
	UServerWidget* ServerWidget;	

	
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

	void SetBehaviorSet(bool PCanMove, bool PCanJump, bool PCanShoot);

	bool GetGodModeState() const;

	void SetGodModeState(bool HasGodMode);

	float GetHealth() const;

	int GetPlayerIndex() const;

	void SetPlayerIndex(int Index);
};
