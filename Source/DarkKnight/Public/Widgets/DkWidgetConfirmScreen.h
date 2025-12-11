// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetConfirmScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class DARKKNIGHT_API UDkWidgetConfirmScreen : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;

	UPROPERTY(meta=(BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons;
};
