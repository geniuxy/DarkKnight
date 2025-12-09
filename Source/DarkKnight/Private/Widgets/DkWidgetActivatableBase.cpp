// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DkWidgetActivatableBase.h"
#include "Controllers/DkUIPlayerController.h"

ADkUIPlayerController* UDkWidgetActivatableBase::GetOwningUIPlayerController()
{
	if (!CachedOwningUIPC.IsValid())
	{
		CachedOwningUIPC = GetOwningPlayer<ADkUIPlayerController>();
	}

	return CachedOwningUIPC.IsValid() ? CachedOwningUIPC.Get() : nullptr;
}
