// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/DkAsyncActionPushSoftWidget.h"

#include "GameplayTagContainer.h"

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

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}
	return nullptr;
}
