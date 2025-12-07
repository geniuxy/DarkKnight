// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DkAsyncActionPushSoftWidget.generated.h"

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
	//~Begin UBlueprintAsyncActionBase Function
	virtual void Activate() override;
	//~End UBlueprintAsyncActionBase Function

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", HidePin = "WorldContextObject",
		BlueprintInternalUseOnly = "true", DisPlayName = "Push Soft Widget To Widget Stack"))
	static UDkAsyncActionPushSoftWidget* PushSoftWidget(
		const UObject* WorldContextObject,
		APlayerController* OwningPlayerController,
		TSoftClassPtr<UDkWidgetActivatableBase> InSoftWidgetClass, // “软类引用”——只记录一个资源路径，等到真正需要时才异步加载
		UPARAM(meta = (Categories = "Dk.WidgetStack")) FGameplayTag InWidgetStackTag,
		bool bFocusOnNewlyPushedWidget = true
	);

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate AfterPush;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	TSoftClassPtr<UDkWidgetActivatableBase> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
};
