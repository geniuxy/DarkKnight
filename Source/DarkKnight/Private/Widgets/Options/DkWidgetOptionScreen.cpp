// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkWidgetOptionScreen.h"

#include "DarkKnightDebugHelper.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"

void UDkWidgetOptionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
		);
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);
}

void UDkWidgetOptionScreen::OnResetBoundActionTriggered()
{
	Debug::Print(TEXT("重置绑定按钮已触发"));
}

void UDkWidgetOptionScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
