// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/DkAsyncActionPushConfirmScreen.h"

#include "Subsytems/DkUISubsystem.h"

void UDkAsyncActionPushConfirmScreen::Activate()
{
	UDkUISubsystem::Get(CachedOwningWorld.Get())->PushConfirmScreenToModalStackAsync(
		CachedScreenType,
		CachedScreenTitle,
		CachedScreenMessage,
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			OnConfirmScreenButtonClicked.Broadcast(ClickedButtonType);

			SetReadyToDestroy();
		}
	);
}

UDkAsyncActionPushConfirmScreen* UDkAsyncActionPushConfirmScreen::PushConfirmScreen(
	const UObject* WorldContextObject,
	EConfirmScreenType ScreenType,
	FText InScreenTitle,
	FText InScreenMessage)
{
	if (GEngine)
	{
		if (UWorld* World =
			GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UDkAsyncActionPushConfirmScreen* Node = NewObject<UDkAsyncActionPushConfirmScreen>();
			Node->CachedOwningWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMessage = InScreenMessage;
			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}

	return nullptr;
}
