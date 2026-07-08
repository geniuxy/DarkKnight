// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetGameHUDScreen.generated.h"

class UAddItemNoticeWidget;
class UTaskTrackingWidget;
class UTaskNoticeWidget;
class UMiniMapWidget;
class UAbilitySystemComponent;
class UValueGauge;
class UCrosshairLockTargetWidget;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkWidgetGameHUDScreen : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

	//~ Begin UCommonActivatableWidget Function
	virtual void NativeOnActivated() override;
	//~ End UCommonActivatableWidget Function

private:
	UPROPERTY(meta=(BindWidget))
	UCrosshairLockTargetWidget* CrosshairLockTargetWidget;
	
	UPROPERTY(meta=(BindWidget))
	UValueGauge* HealthBar;

	UPROPERTY(meta=(BindWidget))
	UValueGauge* EnergyBar;

	UPROPERTY(meta=(BindWidget))
	UMiniMapWidget* MiniMap;

	UPROPERTY(meta=(BindWidget))
	UTaskNoticeWidget* TaskNoticeWidget;

	UPROPERTY(meta=(BindWidget))
	UTaskTrackingWidget* TaskTrackingWidget;

	UPROPERTY(meta=(BindWidget))
	UAddItemNoticeWidget* ItemNoticeWidget;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerAbilitySystemComponent;
};
