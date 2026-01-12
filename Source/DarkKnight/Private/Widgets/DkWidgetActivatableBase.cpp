// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DkWidgetActivatableBase.h"
#include "Controllers/DkUIPlayerController.h"

APlayerController* UDkWidgetActivatableBase::GetOwningPlayerController()
{
	if (!CachedOwningPC.IsValid())
	{
		CachedOwningPC = GetOwningPlayer();
	}

	return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}
