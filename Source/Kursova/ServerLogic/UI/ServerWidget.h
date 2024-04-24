// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "ServerPannelWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBox.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "ServerWidget.generated.h"

class UAdminManagerWidget;

DECLARE_DYNAMIC_DELEGATE_SixParams(FSendSettingsToPlayer, FString, Name, bool, IsPrivate, FString, Password, int, NumberOfPlayers, FString, Map, FString, Difficulty);
DECLARE_DYNAMIC_DELEGATE(FOnFindSession);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnJoinToSession, FBlueprintSessionResult, SessionResult, const FString&, Password);
/**
 * 
 */
UCLASS()
class KURSOVA_API UServerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UEditableText* W_Name;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UEditableText* W_Password;

	// UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	// UComboBox* W_NumberOfPlayers;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* CreateServer;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ConnectToServer;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BackToGame;

	FSendSettingsToPlayer SetServerSettings;

	FOnFindSession OnFindSession;

	FOnJoinToSession OnJoinToSession;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	int dNumberOfPlayers;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	FString SMap;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	FString SDifficulty;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* W_ErrorMessage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCheckBox* IsPrivate;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UScrollBox* SessionsList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UServerPannelWidget> ServerPanelWidgetClass;

	UPROPERTY(EditAnywhere)
	TArray<UServerPannelWidget*> ServerPanelWidgets;
	
	UFUNCTION()
	void ShowErrorMessage(FString Message);

	UFUNCTION()
	void CloseWidget();
	
	UFUNCTION()
	void Create();

	UFUNCTION()
	void Find();

	UFUNCTION()
	void Join(FBlueprintSessionResult SessionResult, const FString& Password);

	UFUNCTION()
	void SetSessions(TArray<FBlueprintSessionResult> BlueprintSessionResults);

	///
	/// New Functionality
	///
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BManageServer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UAdminManagerWidget> AdminManagerWidgetClass;

	UPROPERTY()
	UAdminManagerWidget* AdminManagerWidget;
	
	UFUNCTION()
	void OpenAdminMenu();
	
	UFUNCTION()
	void CloseAdminMenu();
};
