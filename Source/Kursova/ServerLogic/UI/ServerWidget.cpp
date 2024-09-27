// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerWidget.h"
#include "Kursova/MainUI/AdminManagerWidget.h"
#include "Kursova/KursovaGameModeBase.h"

void UServerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	dNumberOfPlayers = 0;
	SMap = "";

	CreateServer->OnClicked.AddDynamic(this, &UServerWidget::Create);
	ConnectToServer->OnClicked.AddDynamic(this, &UServerWidget::Find);
	BackToGame->OnClicked.AddDynamic(this, &UServerWidget::CloseWidget);
	BManageServer->OnClicked.AddDynamic(this, &UServerWidget::OpenAdminMenu);

	if(GetWorld()->GetAuthGameMode())
	{
		BManageServer->SetVisibility(ESlateVisibility::Visible);
	}
}

void UServerWidget::ShowErrorMessage(FString Message)
{
	W_ErrorMessage->SetText(FText::FromString(Message));
}

void UServerWidget::CloseWidget()
{
	GetOwningPlayer()->SetShowMouseCursor(false);
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	RemoveFromParent();
}

void UServerWidget::Create()
{
	FString Errors;
	bool IsAllOk = true;
	
	FString Name;
	if(W_Name->GetText().IsEmpty())
	{
		Name = W_Name->GetHintText().ToString();
	}else{
		Name = W_Name->GetText().ToString();
	}

	FString Password = W_Password->GetText().ToString();
	if(W_Password->GetText().IsEmpty() && IsPrivate->GetCheckedState() == ECheckBoxState::Checked)
	{
		Errors += "Set Password to your private session\n";
		IsAllOk = false;
	}

	if(dNumberOfPlayers == 0)
	{
		Errors += "Choose number of players\n";
		IsAllOk = false;
	}

	if(SMap == "")
	{
		Errors += "Choose map\n";
		IsAllOk = false;
	}
	
	if(IsAllOk)
	{
		SetServerSettings.ExecuteIfBound(Name, IsPrivate->GetCheckedState() == ECheckBoxState::Checked, Password, dNumberOfPlayers, SMap, SDifficulty);
	}else
	{
		ShowErrorMessage(Errors);
	}
}

void UServerWidget::Find()
{
	OnFindSession.ExecuteIfBound();
}

void UServerWidget::Join(FBlueprintSessionResult SessionResult, const FString& Password)
{
	SessionsList->ClearChildren();
	ServerPanelWidgets.Empty();
	OnJoinToSession.ExecuteIfBound(SessionResult, Password);
}

void UServerWidget::SetSessions(TArray<FBlueprintSessionResult> BlueprintSessionResults)
{
	if(ServerPanelWidgetClass)
	{
		for(FBlueprintSessionResult BlueprintSessionResult : BlueprintSessionResults)
		{
			ServerPanelWidgets.Add(CreateWidget<UServerPannelWidget>(GetOwningPlayer(), ServerPanelWidgetClass));
			
			FString Name;
			bool Privacy;
			BlueprintSessionResult.OnlineResult.Session.SessionSettings.Get(FName(TEXT("SessionName")), Name);
			BlueprintSessionResult.OnlineResult.Session.SessionSettings.Get(FName(TEXT("IsPrivate")), Privacy);
			
			ServerPanelWidgets.Last()->Set(Name, Privacy, BlueprintSessionResult);
			SessionsList->AddChild(ServerPanelWidgets.Last());
			ServerPanelWidgets.Last()->OnSessionJoin.BindDynamic(this, &UServerWidget::Join);
		}
	}
}

///
/// New Functionality
///
void UServerWidget::OpenAdminMenu()
{
	APlayerController* Controller = Cast<APlayerController>(GetOwningPlayer());

	if(Controller && AdminManagerWidgetClass)
	{
		AdminManagerWidget = CreateWidget<UAdminManagerWidget>(Controller, AdminManagerWidgetClass);

		if(AdminManagerWidget)
		{
			AdminManagerWidget->OnAdminCloseButtonClicked.BindDynamic(this, &UServerWidget::CloseAdminMenu);
			AdminManagerWidget->SetPlayers();
			AdminManagerWidget->AddToViewport();
		}
	}
}

void UServerWidget::CloseAdminMenu()
{
	AdminManagerWidget->OnAdminCloseButtonClicked.Unbind();
	AdminManagerWidget->RemoveFromParent();
}
