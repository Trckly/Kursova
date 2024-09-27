// Fill out your copyright notice in the Description page of Project Settings.


#include "AdminManagerWidget.h"
#include "../Public/MainPlayer.h"
#include "PlayerPanelWidget.h"
#include "PlayerEditorWidget.h"
#include "Kismet/GameplayStatics.h"

void UAdminManagerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ESearchBar->OnTextChanged.AddDynamic(this, &UAdminManagerWidget::Search);

	if(!BSort->OnClicked.IsBound())
	{
		BSort->OnClicked.AddDynamic(this, &UAdminManagerWidget::SortByCity);
	}

	if(!BEditBehavior->OnClicked.IsBound())
	{
		BEditBehavior->OnClicked.AddDynamic(this, &UAdminManagerWidget::EditPlayerBehavior);
	}

	if(!BClose->OnClicked.IsBound())
	{
		BClose->OnClicked.AddDynamic(this, &UAdminManagerWidget::CloseAdminWidget);
	}

}

void UAdminManagerWidget::SetPlayers()
{
	TArray<AActor*> PlayersArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainPlayer::StaticClass(), PlayersArr);
	
	APlayerController* Controller = Cast<APlayerController>(GetOwningPlayer());
	if(!Controller)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to get PlayerController in AdminManagerWidget"));
		return;
	}

	for(AActor* Player : PlayersArr)
	{
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(Player);
		UPlayerPanelWidget* PanelWidget = CreateWidget<UPlayerPanelWidget>(Controller, PlayerPanelWidgetClass);

		if(PanelWidget)
		{
			PanelWidget->SetWidget(MainPlayer, PlayerIndex);
			PanelWidget->SetWidgetActive.BindDynamic(this, &UAdminManagerWidget::SetActiveWidget);
			MapOfPlayers.Add(PlayerIndex++, PanelWidget);
			SListOfPlayers->AddChild(PanelWidget);
		}
	}
}

void UAdminManagerWidget::Search(const FText& Value)
{	
	if(Value.IsEmpty())
	{
		for(const auto& MapPair : MapOfPlayers)
		{
			MapPair.Value->SetVisibility(ESlateVisibility::Visible);
		}
		return;
	}
	if(OSearchOption->GetSelectedIndex() == 0)
	{
		for(const auto& MapPair : MapOfPlayers)
		{
			if(MapPair.Value->GetName().Contains(Value.ToString()))
			{
				MapPair.Value->SetVisibility(ESlateVisibility::Visible);
			}else
			{
				MapPair.Value->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}else if(OSearchOption->GetSelectedIndex() == 1)
	{
		for(const auto& MapPair : MapOfPlayers)
		{
			if(MapPair.Value->GetCity().Contains(Value.ToString()))
			{
				MapPair.Value->SetVisibility(ESlateVisibility::Visible);
			}else
			{
				MapPair.Value->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UAdminManagerWidget::SortByCity()
{
	for(auto & Element : MapOfPlayers)
	{
		SortArray.Add(Element.Value);
	}

	QuickSortA(SortArray, 0, SortArray.Num() - 1);

	SListOfPlayers->ClearChildren();

	for(int i = 0; i < SortArray.Num(); i++)
	{
		SListOfPlayers->AddChild(SortArray[i]);
	}
	SortArray.Empty();
	
}

void UAdminManagerWidget::EditPlayerBehavior()
{
	APlayerController* Controller = Cast<APlayerController>(GetOwningPlayer());

	if(PlayerEditorWidgetClass && Controller)
	{
		PlayerEditorWidget = CreateWidget<UPlayerEditorWidget>(Controller, PlayerEditorWidgetClass);

		if(PlayerEditorWidget && ActiveWidgetIndex != 0)
		{
			UPlayerPanelWidget* PlayerPanel = *MapOfPlayers.Find(ActiveWidgetIndex);
			PlayerEditorWidget->OnCloseButtonClicked.BindDynamic(this, &UAdminManagerWidget::ClosePlayerEditor);
			PlayerEditorWidget->OnSaveButtonClicked.BindDynamic(this, &UAdminManagerWidget::SavePlayerStats);
			PlayerEditorWidget->SetBehaviorStats(PlayerPanel->GetAbilityToMove(), PlayerPanel->GetAbilityToJump(), PlayerPanel->GetAbilityToFire());
			PlayerEditorWidget->AddToViewport();
		}
	}
}

void UAdminManagerWidget::SetActiveWidget(int Index)
{
	UPlayerPanelWidget* Panel;
	if(ActiveWidgetIndex != 0)
	{
		Panel = *MapOfPlayers.Find(ActiveWidgetIndex);
		Panel->ResetBackgroundColor();
	}
	ActiveWidgetIndex = Index;
	Panel = *MapOfPlayers.Find(ActiveWidgetIndex);
	Panel->SetBackgroundColor();
}

void UAdminManagerWidget::QuickSortA(TArray<UPlayerPanelWidget*>& Array, int Begin, int End)
{

	if(Array.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("There are nothing to sort"));
		return;
	}
	// base case
	if (Begin >= End)
		return;
 
	// partitioning the Array_16Aay
	int p = PartitionA(Array, Begin, End);
 
	// Sorting the left part
	QuickSortA(Array, Begin, p - 1);
 
	// Sorting the right part
	QuickSortA(Array, p + 1, End);
	
}

int UAdminManagerWidget::PartitionA(TArray<UPlayerPanelWidget*>& Array, int Begin, int End)
{
	UPlayerPanelWidget* Pivot = Array[End];
	UPlayerPanelWidget* Temp;
	int i = Begin - 1;
	for(int j = Begin; j <= End - 1; j++)
	{
		if(Array[j]->GetCity() < Pivot->GetCity())
		{
			i++;
			
			Temp = Array[i];
			Array[i] = Array[j];
			Array[j] = Temp;
		}
	}
	i++;
	
	Temp = Array[i];
	Array[i] = Array[End];
	Array[End] = Temp;
 
	return i;
}

void UAdminManagerWidget::ClosePlayerEditor()
{
	PlayerEditorWidget->OnCloseButtonClicked.Unbind();
	PlayerEditorWidget->OnSaveButtonClicked.Unbind();

	PlayerEditorWidget->RemoveFromParent();
}

void UAdminManagerWidget::SavePlayerStats(bool CanMove, bool CanJump, bool CanFire)
{
	if(ActiveWidgetIndex != 0)
	{
		UPlayerPanelWidget* PlayerPanel = *MapOfPlayers.Find(ActiveWidgetIndex);
		PlayerPanel->SetBehavior(CanMove, CanJump, CanFire);
	}
	ClosePlayerEditor();
}

void UAdminManagerWidget::CloseAdminWidget()
{
	for(auto& Panel : MapOfPlayers)
	{
		Panel.Value->SetWidgetActive.Unbind();
	}
	SListOfPlayers->ClearChildren();

	OnAdminCloseButtonClicked.ExecuteIfBound();
}
