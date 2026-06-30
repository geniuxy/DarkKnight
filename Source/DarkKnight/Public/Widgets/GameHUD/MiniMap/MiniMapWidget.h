// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MiniMapWidget.generated.h"

class ADkCharacterBase;
class UCommonTextBlock;
class UCommonLazyImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UMiniMapWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonLazyImage> MiniMapImage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonLazyImage> PlayerIcon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> DistanceText;

	UPROPERTY()
	ADkCharacterBase* OwnerCharacter;
};
