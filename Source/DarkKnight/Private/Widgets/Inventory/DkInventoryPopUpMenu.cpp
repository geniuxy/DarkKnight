// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryPopUpMenu.h"

#include "Components/Slider.h"
#include "Widgets/Components/DkUICommonButtonBase.h"

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
}

void UDkInventoryPopUpMenu::DropButtonClicked()
{
}

void UDkInventoryPopUpMenu::ConsumeButtonClicked()
{
}

void UDkInventoryPopUpMenu::SliderValueChanged(float Value)
{
}
