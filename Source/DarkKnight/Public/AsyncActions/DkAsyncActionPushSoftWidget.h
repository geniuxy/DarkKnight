// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DkAsyncActionPushSoftWidget.generated.h"

struct FGameplayTag;
class UDkWidgetActivatableBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UDkWidgetActivatableBase*, PushedWidget);

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkAsyncActionPushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", HidePin = "WorldContextObject",
		BlueprintInternalUseOnly = "true", DisPlayName = "Push Soft Widget To Widget Stack"))
	static UDkAsyncActionPushSoftWidget* PushSoftWidget(
		const UObject* WorldContextObject,
		APlayerController* OwningPlayerController,
		TSoftClassPtr<UDkWidgetActivatableBase> InSoftWidgetClass,
		UPARAM(meta = (Categories = "Dk.WidgetStack")) FGameplayTag InWidgetStackTag,
		bool bFocusOnNewlyPushedWidget = true
	);

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate AfterPush;
};
