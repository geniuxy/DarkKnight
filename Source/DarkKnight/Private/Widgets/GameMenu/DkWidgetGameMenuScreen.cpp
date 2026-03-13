// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"

#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/GameMenu/CenterAreas/DkGameMenuCenterAreaInventory.h"
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

UDkWidgetInventoryMenu* UDkWidgetGameMenuScreen::GetInventoryMenu() const
{
	return CenterArea_Inventory->GetInventoryMenu();
}

void UDkWidgetGameMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
