// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Interact/DkWidgetDialogScreen.h"

#include "FunctionLibrarys/DkUIFunctionLibrary.h"

void UDkWidgetDialogScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UDkWidgetDialogScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly);
}

void UDkWidgetDialogScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);
}
