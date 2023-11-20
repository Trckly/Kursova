// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "AdminManagerWidget.generated.h"

class AMainPlayer;
class UPlayerPanelWidget;
/**
 * 
 */
UCLASS()
class KURSOVA_API UAdminManagerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UEditableText* ESearchBar;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BSearch;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BSort;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BEditBehavior;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UScrollBox* SListOfPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UPlayerPanelWidget> PlayerPanelWidgetClass;

	UPROPERTY()
	TMap<int, UPlayerPanelWidget*> MapOfPlayers;

	int PlayerIndex;
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetPlayers();

	UFUNCTION()
	void FindByName();

	UFUNCTION()
	void SortByCity();

	UFUNCTION()
	void EditPlayerBehavior();
};
