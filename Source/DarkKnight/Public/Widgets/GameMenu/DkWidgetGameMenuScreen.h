// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetGameMenuScreen.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetGameMenuScreen : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

	//~ Begin UCommonActivatableWidget Function
	virtual void NativeOnDeactivated() override;
	//~ End UCommonActivatableWidget Function

private:
	void OnBackBoundActionTriggered();
};
