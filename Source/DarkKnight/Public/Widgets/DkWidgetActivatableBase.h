// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "DkWidgetActivatableBase.generated.h"

class ADkUIPlayerController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkWidgetActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void LoadLevel(TSoftObjectPtr<UWorld> Level);

protected:
	UFUNCTION(BlueprintPure)
	APlayerController* GetOwningPlayerController();

private:
	TWeakObjectPtr<APlayerController> CachedOwningPC;
};
