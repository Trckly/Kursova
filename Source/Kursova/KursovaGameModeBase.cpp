// Copyright Epic Games, Inc. All Rights Reserved.


#include "KursovaGameModeBase.h"
#include "MainPlayer.h"
#include "UObject/ConstructorHelpers.h"

AKursovaGameModeBase::AKursovaGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MainPlayer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerIndex = 0;
}

void AKursovaGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer != nullptr)
	{
		NewPlayer->SetShowMouseCursor(false);
		NewPlayer->SetInputMode(FInputModeGameOnly());

		AMainPlayer* Player = Cast<AMainPlayer>(NewPlayer->GetCharacter());
		if(Player)
		{
			Player->SetPlayerIndex(PlayerIndex++);
			ArrayOfPlayers.Add(Player);
		}
	}
}

TArray<AMainPlayer*>& AKursovaGameModeBase::GetPlayers()
{
	return ArrayOfPlayers;
}
