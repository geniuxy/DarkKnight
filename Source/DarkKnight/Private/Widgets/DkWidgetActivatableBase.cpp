// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DkWidgetActivatableBase.h"

#include "DarkKnightDebugHelper.h"
#include "Controllers/DkUIPlayerController.h"

void UDkWidgetActivatableBase::LoadLevel(TSoftObjectPtr<UWorld> Level)
{
	const FName LevelURL = FName(*FPackageName::ObjectPathToPackageName(Level.ToString()));

	if (LevelURL != "")
	{
		FString TravelStr = FString::Printf(TEXT("%s?listen"), *LevelURL.ToString());
		Debug::Print(FString::Printf(TEXT("Server traveling to: %s"), *TravelStr));
		GetWorld()->ServerTravel(TravelStr);
	}
}

APlayerController* UDkWidgetActivatableBase::GetOwningPlayerController()
{
	if (!CachedOwningPC.IsValid())
	{
		CachedOwningPC = GetOwningPlayer();
	}

	return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}
