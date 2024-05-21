// Fill out your copyright notice in the Description page of Project Settings.


#include "SubscriptionManager.h"

void USubscriptionManager::Subscribe(IEventListener* NewListener)
{
	Listeners.Add(NewListener);
}

void USubscriptionManager::Unsubscribe(IEventListener* NewListener)
{
	Listeners.Remove(NewListener);
}

void USubscriptionManager::Notify(float Value)
{
	for (const auto Listener : Listeners)
	{
		Listener->Update(Value);
	}
}
