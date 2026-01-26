// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkInventoryPopUpMenu.generated.h"

class UCommonTextBlock;
class USlider;
class USizeBox;
class UDkUICommonButtonBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryPopUpMenu : public UCommonUserWidget
{
	GENERATED_BODY()

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USlider> Slider_Split;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> Text_SplitAmount;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDkUICommonButtonBase> Button_Split;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDkUICommonButtonBase> Button_Drop;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDkUICommonButtonBase> Button_Consume;
	//***** Bound Widgets *****//
};
