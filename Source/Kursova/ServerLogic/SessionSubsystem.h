// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SessionSubsystem.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EBPOnJoinSessionCompleteResult : uint8
{
	Success,
	SessionIsFull,
	SessionDoesNotExist,
	CouldNotRetrieveAddress,
	AlreadyInSession,
	UnknownError
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnDestroySessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCSOnFindSessionsComplete, const TArray<FBlueprintSessionResult>&, SessionsResult, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnJoinSessionComplete, EBPOnJoinSessionCompleteResult, Result);

UCLASS()
class USessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USessionSubsystem();

	UFUNCTION(BlueprintCallable) void CreateSession(int32 NumPublicConnections, bool IsLANMatch, FString SessionName, bool IsPrivate = false, FString SessionPassword = "", FString Map = "Undefined", FString Difficulty = "Medium");
	UFUNCTION(BlueprintCallable) void UpdateSession();
	UFUNCTION(BlueprintCallable) void StartSession();
	UFUNCTION(BlueprintCallable) void EndSession();
	UFUNCTION(BlueprintCallable) void DestroySession();
	UFUNCTION(BlueprintCallable) void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
	UFUNCTION(BlueprintCallable) void JoinGameSession(const FBlueprintSessionResult& SessionResult, const FString& Password);
	UFUNCTION(BlueprintCallable) bool SessionHasBeenStarted();
	
	UPROPERTY(BlueprintAssignable) FCSOnCreateSessionComplete OnCreateSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable) FCSOnUpdateSessionComplete OnUpdateSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable) FCSOnStartSessionComplete OnStartSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable) FCSOnEndSessionComplete OnEndSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable) FCSOnDestroySessionComplete OnDestroySessionCompleteEvent;
	UPROPERTY(BlueprintAssignable) FCSOnFindSessionsComplete OnFindSessionsCompleteEvent;
	UPROPERTY(BlueprintAssignable) FCSOnJoinSessionComplete OnJoinGameSessionCompleteEvent;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnEndSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession();
	

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
  	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
