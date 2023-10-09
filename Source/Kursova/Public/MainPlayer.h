// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

DECLARE_DELEGATE(FRackDelegate)

UCLASS()
class KURSOVA_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InteractDistance = 200.f;

	TSubclassOf<AActor*> RackClass;

	FVector PreviousActorLocation;
	FRotator PreviousActorRotation;

	FVector WeaponChooseLocation = FVector(200.f, -810.f, 152.f);
	FRotator WeaponChooseRotation = FRotator(0.f, -90.f, 0.f);
	
	UPROPERTY(BlueprintReadOnly)
	bool bShowCrosshair = true;

protected:
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

	FRackDelegate RackDelegate;
};
