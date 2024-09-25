// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventListener.h"
#include "SubscriptionManager.generated.h"

/**
 * 
 */
UCLASS()
class KURSOVA_API USubscriptionManager : public UObject
{
	GENERATED_BODY()

protected:
	TArray<IEventListener*> Listeners;

public:
	void Subscribe(IEventListener* NewListener);
	void Unsubscribe(IEventListener* NewListener);
	void Notify(float Value);
};
