// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "PasswordWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPasswordCommit,const FString&, Password);
/**
 * 
 */
UCLASS()
class KURSOVA_API UPasswordWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* W_Commit;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UEditableText* W_Password;

	FOnPasswordCommit OnPasswordCommit;
	
	UFUNCTION()
	void CommitPassword();
};
