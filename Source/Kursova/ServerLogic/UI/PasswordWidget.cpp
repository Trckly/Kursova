// Fill out your copyright notice in the Description page of Project Settings.


#include "PasswordWidget.h"

void UPasswordWidget::NativeConstruct()
{
	Super::NativeConstruct();

	W_Commit->OnClicked.AddDynamic(this, &UPasswordWidget::CommitPassword);
}

void UPasswordWidget::CommitPassword()
{
	if(!W_Password->GetText().IsEmpty())
	{
		OnPasswordCommit.ExecuteIfBound(W_Password->GetText().ToString());
	}
}
