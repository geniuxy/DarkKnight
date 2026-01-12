// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"

#include "ICommonInputModule.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Input/CommonUIInputTypes.h"

void UDkWidgetGameMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);
}

void UDkWidgetGameMenuScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);
}

void UDkWidgetGameMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
