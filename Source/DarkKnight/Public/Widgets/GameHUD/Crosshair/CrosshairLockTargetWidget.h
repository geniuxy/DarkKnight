// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CrosshairLockTargetWidget.generated.h"

class UCanvasPanelSlot;
struct FGameplayTag;
class UCommonLazyImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UCrosshairLockTargetWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* CrosshairImage;

	void CrosshairTagUpdated(const FGameplayTag Tag, int32 NewCount);

	UPROPERTY()
	UCanvasPanelSlot* CrosshairCanvasPanelSlot;

	UPROPERTY()
	APlayerController* CachedPlayerController;

	void UpdateCrosshairPosition();
};
