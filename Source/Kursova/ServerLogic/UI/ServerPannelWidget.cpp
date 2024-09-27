// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerPannelWidget.h"

void UServerPannelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	W_SelectServer->OnClicked.AddDynamic(this, &UServerPannelWidget::SelectServer);
}

void UServerPannelWidget::Set(const FString& ServerName, bool IsPrivate, const FBlueprintSessionResult& BPSessionResult)
{
	if(!ServerName.IsEmpty())
	{
		W_ServerName->SetText(FText::FromString(ServerName));
		SessionResult = BPSessionResult;

		IsPrivateServer = IsPrivate;
		if(IsPrivateServer)
		{
			W_Privacy->SetText(FText::FromString("Private"));
		}else
		{
			W_Privacy->SetText(FText::FromString("Public"));
		}
	}
}

void UServerPannelWidget::SendPassword(const FString& Password)
{
	PasswordWidget->RemoveFromParent();
	OnSessionJoin.ExecuteIfBound(SessionResult, Password);
}

void UServerPannelWidget::SelectServer()
{
	if(!IsPrivateServer)
	{
		OnSessionJoin.ExecuteIfBound(SessionResult, "");
	}else
	{
		if(PasswordWidgetClass)
		{
			PasswordWidget = CreateWidget<UPasswordWidget>(GetOwningPlayer(), PasswordWidgetClass);
			if(PasswordWidget)
			{
				PasswordWidget->OnPasswordCommit.BindDynamic(this, &UServerPannelWidget::SendPassword);
				PasswordWidget->AddToViewport();
			}
		}
	}
}
