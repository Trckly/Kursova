// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "FindSessionsCallbackProxy.h"
#include "PasswordWidget.h"
#include "ServerPannelWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnSessionJoin, FBlueprintSessionResult, SessionResult, const FString&, Password);
/**
 * 
 */
UCLASS()
class KURSOVA_API UServerPannelWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* W_ServerName;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* W_Privacy;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* W_SelectServer;

	UPROPERTY()
	FBlueprintSessionResult SessionResult;
	
	bool IsPrivateServer;
	
	FOnSessionJoin OnSessionJoin;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPasswordWidget> PasswordWidgetClass;

	UPROPERTY(EditAnywhere)
	UPasswordWidget* PasswordWidget;

	UFUNCTION()
	void Set(const FString& ServerName, bool IsPrivate, const FBlueprintSessionResult& BPSessionResult);

	UFUNCTION()
	void SendPassword(const FString& Password);

	UFUNCTION()
	void SelectServer();
};
