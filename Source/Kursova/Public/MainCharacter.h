// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "MainCharacter.generated.h"

UCLASS()
class KURSOVA_API AMainCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CharacterHeight = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 100.f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Scale);

	void MoveRight(float Scale);
};
