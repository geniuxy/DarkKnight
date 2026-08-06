// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DarkKnight/DarkKnight.h"
#include "DkInventoryPopUpMenu.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FPopUpMenuSplit, int32, SplitAmount, int32, Index);

DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuDrop, int32, Index);

DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuConsume, int32, Index);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FPopUpMenuSell, int32, SellAmount, int32, Index);

class UCommonTextBlock;
class USlider;
class USizeBox;
class UDkUICommonButtonBase;
/**
 * 背包物品右键后弹出的菜单
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryPopUpMenu : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	//~Begin UUserWidget Function
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	//~End UUserWidget Function
	
	FPopUpMenuSplit OnSplit;
	FPopUpMenuDrop OnDrop;
	FPopUpMenuConsume OnConsume;
	FPopUpMenuSell OnSell;

	int32 GetSplitAmount() const;

	void CollapseSplitButton() const;
	void CollapseSellButton() const;
	void CollapseConsumeButton() const;

	void SetSliderParams(const float Max, const float Value) const;

	FVector2D GetBoxSize() const;

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

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UDkUICommonButtonBase> Button_Sell;
	//***** Bound Widgets *****//

	void SplitButtonClicked();

	UFUNCTION()
	void DropButtonClicked();

	UFUNCTION()
	void ConsumeButtonClicked();

	UFUNCTION()
	void SellButtonClicked();

	UFUNCTION()
	void SliderValueChanged(float Value);

	int32 GridIndex = INDEX_NONE;

public:
	LIST_DATA_ACCESSOR(int32, GridIndex)
};
