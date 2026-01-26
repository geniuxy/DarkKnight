// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryPopUpMenu.h"

#include "CommonTextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Widgets/Components/DkUICommonButtonBase.h"

int32 UDkInventoryPopUpMenu::GetSplitAmount() const
{
	return FMath::Floor(Slider_Split->GetValue());
}

void UDkInventoryPopUpMenu::CollapseSplitButton() const
{
	Button_Split->SetVisibility(ESlateVisibility::Collapsed);
	Slider_Split->SetVisibility(ESlateVisibility::Collapsed);
	Text_SplitAmount->SetVisibility(ESlateVisibility::Collapsed);
}

void UDkInventoryPopUpMenu::CollapseConsumeButton() const
{
	Button_Consume->SetVisibility(ESlateVisibility::Collapsed);
}

void UDkInventoryPopUpMenu::SetSliderParams(const float Max, const float Value) const
{
	Slider_Split->SetMaxValue(Max);
	Slider_Split->SetMinValue(1);
	Slider_Split->SetValue(Value);
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}

FVector2D UDkInventoryPopUpMenu::GetBoxSize() const
{
	return FVector2D(SizeBox_Root->GetWidthOverride(), SizeBox_Root->GetHeightOverride());
}

void UDkInventoryPopUpMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Slider_Split->OnValueChanged.AddDynamic(this, &ThisClass::SliderValueChanged);
	Button_Split->OnClicked().AddUObject(this, &ThisClass::SplitButtonClicked);
	Button_Drop->OnClicked().AddUObject(this, &ThisClass::DropButtonClicked);
	Button_Consume->OnClicked().AddUObject(this, &ThisClass::ConsumeButtonClicked);
}

void UDkInventoryPopUpMenu::SplitButtonClicked()
{
	if (OnSplit.ExecuteIfBound(GetSplitAmount(), GridIndex))
	{
		RemoveFromParent();
	}
}

void UDkInventoryPopUpMenu::DropButtonClicked()
{
	if (OnDrop.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UDkInventoryPopUpMenu::ConsumeButtonClicked()
{
	if (OnConsume.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UDkInventoryPopUpMenu::SliderValueChanged(float Value)
{
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}
