// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/DkAsyncActionPushSoftWidget.h"

#include "Subsytems/DkUISubsystem.h"
#include "Widgets/DkWidgetActivatableBase.h"

void UDkAsyncActionPushSoftWidget::Activate()
{
	UDkUISubsystem* UISubsystem = UDkUISubsystem::Get(CachedOwningWorld.Get());

	checkf(UISubsystem, TEXT("UISubsystem为空！"));

	UISubsystem->PushSoftWidgetToStackAsync(
		CachedWidgetStackTag, CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());

				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
				break;
			case EAsyncPushWidgetState::AfterPush:

				AfterPush.Broadcast(PushedWidget);

				if (bCachedFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}

				SetReadyToDestroy();

				break;
			}
		}
	);
}

UDkAsyncActionPushSoftWidget* UDkAsyncActionPushSoftWidget::PushSoftWidget(
	const UObject* WorldContextObject,
	APlayerController* OwningPlayerController,
	TSoftClassPtr<UDkWidgetActivatableBase> InSoftWidgetClass,
	FGameplayTag InWidgetStackTag,
	bool bFocusOnNewlyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack需要一个非空的入参InSoftWidgetClass"));

	if (GEngine)
	{
		if (UWorld* World =
			GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UDkAsyncActionPushSoftWidget* Node = NewObject<UDkAsyncActionPushSoftWidget>();

			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;
			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}
	return nullptr;
}
