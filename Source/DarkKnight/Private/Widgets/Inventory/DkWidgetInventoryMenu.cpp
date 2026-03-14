// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkWidgetInventoryMenu.h"

#include "CommonTextBlock.h"
#include "DarkKnightDebugHelper.h"
#include "Components/DkItemComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Widgets/Inventory/DkInventoryItemGrid.h"
#include "Widgets/Components/Buttons/DkUICommonButtonBase.h"
#include "Widgets/Components/Buttons/DkUICommonButtonImage.h"

FDkInventorySlotAvailabilityResult UDkWidgetInventoryMenu::HasRoomForItem(UDkItemComponent* ItemComponent) const
{
	switch (ItemComponent->GetItemManifest().GetItemCategory())
	{
	case EInventoryItemCategory::Equipment:
		return GridEquipments->HasRoomForItem(ItemComponent);
	case EInventoryItemCategory::Consumable:
		return GridConsumables->HasRoomForItem(ItemComponent);
	case EInventoryItemCategory::CraftingMaterial:
		return GridCraftingMaterials->HasRoomForItem(ItemComponent);
	case EInventoryItemCategory::None:
		Debug::Print(TEXT("ItemComponent没有配置ItemCategory"));
		return FDkInventorySlotAvailabilityResult();
	}
	return FDkInventorySlotAvailabilityResult();
}

FDkInventorySlotAvailabilityResult UDkWidgetInventoryMenu::HasRoomForItem(UDkInventoryItem* Item) const
{
	switch (Item->GetItemManifest().GetItemCategory())
	{
	case EInventoryItemCategory::Equipment:
		return GridEquipments->HasRoomForItem(Item);
	case EInventoryItemCategory::Consumable:
		return GridConsumables->HasRoomForItem(Item);
	case EInventoryItemCategory::CraftingMaterial:
		return GridCraftingMaterials->HasRoomForItem(Item);
	case EInventoryItemCategory::None:
		Debug::Print(TEXT("Item没有配置ItemCategory"));
		return FDkInventorySlotAvailabilityResult();
	}
	return FDkInventorySlotAvailabilityResult();
}

void UDkWidgetInventoryMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Equipment->OnClicked().AddUObject(this, &ThisClass::ShowEquipments);
	Button_Consumable->OnClicked().AddUObject(this, &ThisClass::ShowConsumables);
	Button_CraftingMaterial->OnClicked().AddUObject(this, &ThisClass::ShowCraftingMaterials);

	CategoryButtonMap.Add(EInventoryItemCategory::Equipment, Button_Equipment);
	CategoryButtonMap.Add(EInventoryItemCategory::Consumable, Button_Consumable);
	CategoryButtonMap.Add(EInventoryItemCategory::CraftingMaterial, Button_CraftingMaterial);

	SelectedUnderlineMap.Add(EInventoryItemCategory::Equipment, SelectedEquipmentUnderline);
	SelectedUnderlineMap.Add(EInventoryItemCategory::Consumable, SelectedConsumableUnderline);
	SelectedUnderlineMap.Add(EInventoryItemCategory::CraftingMaterial, SelectedCraftingMaterialUnderline);

	ShowEquipments();
}

void UDkWidgetInventoryMenu::ShowEquipments()
{
	SetActiveGrid(GridEquipments, Button_Equipment);
}

void UDkWidgetInventoryMenu::ShowConsumables()
{
	SetActiveGrid(GridConsumables, Button_Consumable);
}

void UDkWidgetInventoryMenu::ShowCraftingMaterials()
{
	SetActiveGrid(GridCraftingMaterials, Button_CraftingMaterial);
}

void UDkWidgetInventoryMenu::SelectButton(UDkUICommonButtonImage* Button)
{
	for (TTuple<EInventoryItemCategory, TObjectPtr<UDkUICommonButtonImage>> Pair : CategoryButtonMap)
	{
		Pair.Value->ToggleHighlightState(false);
	}
	Button->ToggleHighlightState(true);
}

void UDkWidgetInventoryMenu::ShowSelectedUnderline(UDkInventoryItemGrid* Grid)
{
	for (TTuple<EInventoryItemCategory, TObjectPtr<UCommonLazyImage>> Pair : SelectedUnderlineMap)
	{
		Pair.Value->SetVisibility(ESlateVisibility::Hidden);
	}
	SelectedUnderlineMap[Grid->GetItemCategory()]->SetVisibility(ESlateVisibility::Visible);
}

void UDkWidgetInventoryMenu::SetActiveGrid(UDkInventoryItemGrid* Grid, UDkUICommonButtonImage* Button)
{
	SelectButton(Button);

	ActiveGrid = Grid;

	ShowSelectedUnderline(Grid);

	Switcher->SetActiveWidget(Grid);

	switch (Grid->GetItemCategory())
	{
	case EInventoryItemCategory::Equipment:
		InventoryTitleTxt->SetText(FText::FromString(TEXT("装备")));
		break;
	case EInventoryItemCategory::Consumable:
		InventoryTitleTxt->SetText(FText::FromString(TEXT("消耗品")));
		break;
	case EInventoryItemCategory::CraftingMaterial:
		InventoryTitleTxt->SetText(FText::FromString(TEXT("合成材料")));
		break;
	case EInventoryItemCategory::None:
		break;
	}
}
