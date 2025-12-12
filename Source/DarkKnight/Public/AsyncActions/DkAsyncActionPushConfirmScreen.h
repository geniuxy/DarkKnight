// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkEnums.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DkAsyncActionPushConfirmScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedDelegate,
                                            EConfirmScreenButtonType, ClickedButtonType);

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkAsyncActionPushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//~Begin UBlueprintAsyncActionBase Function
	virtual void Activate() override;
	//~End UBlueprintAsyncActionBase Function

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject",
		BlueprintInternalUseOnly = "true", DisplayName = "Show Confirm Screen"))
	static UDkAsyncActionPushConfirmScreen* PushConfirmScreen(
		const UObject* WorldContextObject,
		EConfirmScreenType ScreenType,
		FText InScreenTitle,
		FText InScreenMessage
	);

	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnConfirmScreenButtonClicked;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EConfirmScreenType CachedScreenType;
	FText CachedScreenTitle;
	FText CachedScreenMessage;
};
