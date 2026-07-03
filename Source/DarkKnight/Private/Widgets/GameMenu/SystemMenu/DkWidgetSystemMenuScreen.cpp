// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/SystemMenu/DkWidgetSystemMenuScreen.h"

#include "DkGameplayTags.h"
#include "ICommonInputModule.h"
#include "DkTypes/DkEnums.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/Components/Buttons/DkUICommonButtonImage.h"
#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"

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

	SelectButton_Inventory->OnClicked().AddUObject(this, &ThisClass::HandleSelectButtonInventory);
	SelectButton_Map->OnClicked().AddUObject(this, &ThisClass::HandleSelectButtonMap);
	SelectButton_Task->OnClicked().AddUObject(this, &ThisClass::HandleSelectButtonTask);
}

void UDkWidgetSystemMenuScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly, false);
}

void UDkWidgetSystemMenuScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);
}

void UDkWidgetSystemMenuScreen::HandleSelectButtonInventory()
{
	UDkUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_GameMenu,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_GameMenu),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly, false);
			}
			if (InPushState == EAsyncPushWidgetState::AfterPush)
			{
				UDkWidgetGameMenuScreen* GameMenuScreen = CastChecked<UDkWidgetGameMenuScreen>(PushedWidget);
				GameMenuScreen->SetVisibleCenterArea(DkGameplayTags::Dk_Widget_GameMenu_Inventory);
			}
		}
	);
}

void UDkWidgetSystemMenuScreen::HandleSelectButtonMap()
{
	UDkUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_GameMenu,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_GameMenu),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly, false);
			}
			if (InPushState == EAsyncPushWidgetState::AfterPush)
			{
				UDkWidgetGameMenuScreen* GameMenuScreen = CastChecked<UDkWidgetGameMenuScreen>(PushedWidget);
				GameMenuScreen->SetVisibleCenterArea(DkGameplayTags::Dk_Widget_GameMenu_Map);
			}
		}
	);
}

void UDkWidgetSystemMenuScreen::HandleSelectButtonTask()
{
	UDkUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_GameMenu,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_GameMenu),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly, false);
			}
			if (InPushState == EAsyncPushWidgetState::AfterPush)
			{
				UDkWidgetGameMenuScreen* GameMenuScreen = CastChecked<UDkWidgetGameMenuScreen>(PushedWidget);
				GameMenuScreen->SetVisibleCenterArea(DkGameplayTags::Dk_Widget_GameMenu_Task);
			}
		}
	);
}

void UDkWidgetSystemMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
