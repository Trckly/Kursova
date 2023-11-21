// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "AdminManagerWidget.generated.h"

class UPlayerEditorWidget;
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
	UButton* BSort;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BEditBehavior;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UScrollBox* SListOfPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UPlayerPanelWidget> PlayerPanelWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UPlayerEditorWidget> PlayerEditorWidgetClass;

	UPROPERTY()
	UPlayerEditorWidget* PlayerEditorWidget;

	UPROPERTY()
	TMap<int, UPlayerPanelWidget*> MapOfPlayers;

	UPROPERTY()
	int PlayerIndex = 1;

	UPROPERTY()
	int ActiveWidgetIndex = 0;

	UPROPERTY()
	TArray<UPlayerPanelWidget*> SortArray;
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetPlayers();

	UFUNCTION()
	void FindByName(const FText& Value);

	UFUNCTION()
	void SortByCity();

	UFUNCTION()
	void EditPlayerBehavior();

	UFUNCTION()
	void SetActiveWidget(int Index);

	void QuickSortA(TArray<UPlayerPanelWidget*>& Array, int Begin, int End);

	int PartitionA(TArray<UPlayerPanelWidget*>& Array, int Begin, int End);

	UFUNCTION()
	void ClosePlayerEditor();

	UFUNCTION()
	void SavePlayerStats(bool CanMove, bool CanJump, bool CanFire);
};
