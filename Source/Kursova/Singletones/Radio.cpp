// Fill out your copyright notice in the Description page of Project Settings.


#include "Radio.h"

ARadio* ARadio::InitializeRadio()
{
	if(SelfInstance == nullptr)
	{
		SelfInstance = CreateDefaultSubobject<ARadio>(TEXT("Radio"));
	}

	return SelfInstance;
}
