// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkInventoryPopUpMenu.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FPopUpMenuSplit, int32, SplitAmount, int32, Index);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuDrop, int32, Index);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuConsume, int32, Index);

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

protected:
	FPopUpMenuSplit OnSplit;
	FPopUpMenuDrop OnDrop;
	FPopUpMenuConsume OnConsume;

	int32 GetSplitAmount() const;

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

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

	void SplitButtonClicked();

	UFUNCTION()
	void DropButtonClicked();

	UFUNCTION()
	void ConsumeButtonClicked();

	UFUNCTION()
	void SliderValueChanged(float Value);

	int32 GridIndex = INDEX_NONE;
};
