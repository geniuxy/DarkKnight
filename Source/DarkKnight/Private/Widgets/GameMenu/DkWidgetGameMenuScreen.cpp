// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"

#include "CommonVisibilitySwitcher.h"
#include "DkGameplayTags.h"
#include "ICommonInputModule.h"
#include "DkTypes/DkEnums.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/GameMenu/CenterAreas/DkGameMenuCenterAreaInventory.h"
#include "Widgets/GameMenu/CenterAreas/DkGameMenuCenterAreaTask.h"
#include "Widgets/Inventory/DkInventoryDraggedItem.h"

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

void UDkWidgetGameMenuScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly, false);
}

void UDkWidgetGameMenuScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (UDkUISubsystem::Get(this)->IsWidgetStackEmpty(DkGameplayTags::Dk_WidgetStack_GameMenu))
	{
		UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);
	}
}

UDkWidgetInventoryMenu* UDkWidgetGameMenuScreen::GetInventoryMenu() const
{
	return CenterArea_Inventory->GetInventoryMenu();
}

void UDkWidgetGameMenuScreen::SetVisibleCenterArea(const FGameplayTag& InTag) const
{
	if (UDkWidgetActivatableBase* CenterAreaWidget = GetActivatableCenterAreaWidget(InTag))
	{
		CommonVisibilitySwitcher->SetActiveWidget(CenterAreaWidget);
	}
}

UDkWidgetActivatableBase* UDkWidgetGameMenuScreen::GetActivatableCenterAreaWidget(const FGameplayTag& InTag) const
{
	if (InTag.MatchesTagExact(DkGameplayTags::Dk_Widget_GameMenu_Inventory))
	{
		return CenterArea_Inventory;
	}
	if (InTag.MatchesTagExact(DkGameplayTags::Dk_Widget_GameMenu_Task))
	{
		return CenterArea_Task;
	}
	if (InTag.MatchesTagExact(DkGameplayTags::Dk_Widget_GameMenu_Map))
	{
		return CenterArea_Map;
	}
	return nullptr;
}

void UDkWidgetGameMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
