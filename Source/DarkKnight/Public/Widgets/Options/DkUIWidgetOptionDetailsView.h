// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DkUIWidgetOptionDetailsView.generated.h"

class UCommonTextBlock;
class UCommonLazyImage;
class UCommonRichTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkUIWidgetOptionDetailsView : public UUserWidget
{
	GENERATED_BODY()

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* CommonLazyImage_DescriptionImage;

	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichText_Description;

	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichText_DynamicDetails;

	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichText_DisabledReason;
	//***** Bound Widgets *****//
};
