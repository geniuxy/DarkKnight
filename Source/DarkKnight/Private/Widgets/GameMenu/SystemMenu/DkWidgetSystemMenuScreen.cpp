// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/SystemMenu/DkWidgetSystemMenuScreen.h"

#include "ICommonInputModule.h"
#include "DkTypes/DkEnums.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Input/CommonUIInputTypes.h"

void UDkWidgetSystemMenuScreen::NativeOnInitialized()
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

void UDkWidgetSystemMenuScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);
}

void UDkWidgetSystemMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
