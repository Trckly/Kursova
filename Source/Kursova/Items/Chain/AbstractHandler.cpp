// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractHandler.h"

IHandler* UAbstractHandler::SetNext(IHandler* Handler)
{
	NextHandler = Handler;

	return Handler;
}

bool UAbstractHandler::Handle(AAbstractItem* PickUpItem)
{
	if(NextHandler)
		return NextHandler->Handle(PickUpItem);
	return false;
}
