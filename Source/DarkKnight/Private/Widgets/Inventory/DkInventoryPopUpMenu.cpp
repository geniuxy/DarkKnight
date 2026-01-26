// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryPopUpMenu.h"

#include "Components/Slider.h"
#include "Widgets/Components/DkUICommonButtonBase.h"

int32 UDkInventoryPopUpMenu::GetSplitAmount() const
{
	return FMath::Floor(Slider_Split->GetValue());
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
}
