// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/DkWidgetGameHUDScreen.h"

#include "FunctionLibrarys/DkUIFunctionLibrary.h"

void UDkWidgetGameHUDScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bSupportsActivationFocus = false; // 该设置可使Widget不会阻止鼠标等输入
}

void UDkWidgetGameHUDScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);
}
