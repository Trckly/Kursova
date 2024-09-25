// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::SetHealth(float Health)
{
	PHealth->SetPercent(Health / 100.f);
}

void UPlayerHUD::SetArmor(float Armor)
{
	PArmor->SetPercent(Armor / 100.f);
}
