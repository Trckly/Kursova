// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/KismetSystemLibrary.h"

USessionSubsystem::USessionSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted))
	, UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted))
	, StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompleted))
	, EndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnEndSessionCompleted))
	, DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted))
	, FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted))
{
}

bool USessionSubsystem::SessionHasBeenStarted()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	//EOnlineSessionState::Type SessionState = SessionInterface->GetSessionState(NAME_GameSession);
	bool bIsValidSession = SessionInterface.IsValid();

	if (bIsValidSession)
	{
		EOnlineSessionState::Type SessionState = SessionInterface->GetSessionState(NAME_GameSession);
		return SessionState >= EOnlineSessionState::Pending;
	}
	else
	{
		return false; // або що потрібно повернути, якщо сеанс недійсний
	}
	
	//return SessionState >= EOnlineSessionState::Pending();
}

void USessionSubsystem::CreateSession(int32 NumPublicConnections, bool IsLANMatch, FString SessionName, bool IsPrivate, FString SessionPassword, FString Map, FString Difficulty)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	LastSessionSettings = MakeShared<FOnlineSessionSettings>();
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = IsLANMatch;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->Set(FName(TEXT("SessionName")), FString(SessionName), EOnlineDataAdvertisementType::ViaOnlineService);
	LastSessionSettings->Set(FName(TEXT("IsPrivate")), bool(IsPrivate), EOnlineDataAdvertisementType::ViaOnlineService);
	LastSessionSettings->Set(FName(TEXT("Password")), FString(SessionPassword), EOnlineDataAdvertisementType::ViaOnlineService);

	LastSessionSettings->Set(FName(TEXT("MAPNAME")), FString(Map), EOnlineDataAdvertisementType::ViaOnlineService);

	LastSessionSettings->Set(FName(TEXT("Difficulty")), FString(Difficulty), EOnlineDataAdvertisementType::ViaOnlineService);

	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void USessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	OnCreateSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::UpdateSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}

	TSharedPtr<FOnlineSessionSettings> UpdatedSessionSettings = MakeShared<FOnlineSessionSettings>(*LastSessionSettings);
	UpdatedSessionSettings->Set("MAPNAME", FString("Updated Level Name"), EOnlineDataAdvertisementType::ViaOnlineService);

	UpdateSessionCompleteDelegateHandle =
		SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

	if (!SessionInterface->UpdateSession(NAME_GameSession, *UpdatedSessionSettings))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);

		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		LastSessionSettings = UpdatedSessionSettings;
	}
}

void USessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}

	OnUpdateSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::StartSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnStartSessionCompleteEvent.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle =
		SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

	if (!SessionInterface->StartSession(NAME_GameSession))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void USessionSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	if (Successful)
	{
		FString MapName;
		SessionInterface->GetSessionSettings(SessionName)->Get("MAPNAME", MapName);
		if(MapName != "Undefined")
		{
			FString ConnectString = MapName + FString("Map?listen");
			if (!GetWorld()->ServerTravel(ConnectString, true))
			{
				UKismetSystemLibrary::PrintString(this, FString("Error: server travel"));
			}
		}else
		{
			UKismetSystemLibrary::PrintString(this, FString("Error: server travel"));
		}
	}

	OnStartSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::EndSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnEndSessionCompleteEvent.Broadcast(false);
		return;
	}

	EndSessionCompleteDelegateHandle =
		SessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);

	if (!SessionInterface->EndSession(NAME_GameSession))
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);

		OnEndSessionCompleteEvent.Broadcast(false);
	}
}

void USessionSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
	}

	OnEndSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::DestroySession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnDestroySessionCompleteEvent.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle =
		SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(NAME_GameSession))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

		OnDestroySessionCompleteEvent.Broadcast(false);
	}
}

void USessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	OnDestroySessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FBlueprintSessionResult>(), false);
		return;
	}

	FindSessionsCompleteDelegateHandle =
		SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShared<FOnlineSessionSearch>();
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;

	LastSessionSearch->QuerySettings.Set(FName(TEXT("PRESENCESEARCH")), true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		OnFindSessionsCompleteEvent.Broadcast(TArray<FBlueprintSessionResult>(), false);
	}
}

void USessionSubsystem::OnFindSessionsCompleted(bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FBlueprintSessionResult>(), Successful);
		return;
	}

	TArray<FBlueprintSessionResult> BlueprintSessionResults;
	for (const FOnlineSessionSearchResult El : LastSessionSearch->SearchResults)
	{
		FBlueprintSessionResult BlueprintSessionResult;
		BlueprintSessionResult.OnlineResult = El;
		BlueprintSessionResults.Add(BlueprintSessionResult);
	}

	OnFindSessionsCompleteEvent.Broadcast(BlueprintSessionResults, Successful);
}

void USessionSubsystem::JoinGameSession(const FBlueprintSessionResult& SessionResult, const FString& Password)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnJoinGameSessionCompleteEvent.Broadcast(static_cast<EBPOnJoinSessionCompleteResult>(EOnJoinSessionCompleteResult::UnknownError));
		return;
	}

	bool IsPrivate;
	SessionResult.OnlineResult.Session.SessionSettings.Get(FName(TEXT("IsPrivate")), IsPrivate);
	
	if(IsPrivate)
	{
		FString SessionPassword;
		SessionResult.OnlineResult.Session.SessionSettings.Get(FName(TEXT("Password")), SessionPassword);
		if(SessionPassword != Password)
		{
			OnJoinGameSessionCompleteEvent.Broadcast(static_cast<EBPOnJoinSessionCompleteResult>(EOnJoinSessionCompleteResult::UnknownError));
			return;
		}
	}
	JoinSessionCompleteDelegateHandle =
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult.OnlineResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		OnJoinGameSessionCompleteEvent.Broadcast(static_cast<EBPOnJoinSessionCompleteResult>(EOnJoinSessionCompleteResult::UnknownError));
	}
}

void USessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		TryTravelToCurrentSession();
	}

	OnJoinGameSessionCompleteEvent.Broadcast(static_cast<EBPOnJoinSessionCompleteResult>(Result));
}

bool USessionSubsystem::TryTravelToCurrentSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	FString ConnectString;
	if (!SessionInterface->GetResolvedConnectString(NAME_GameSession, ConnectString))
	{
		return false;
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(ConnectString, TRAVEL_Absolute);
	return true;
}