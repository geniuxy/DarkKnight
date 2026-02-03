// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DKWidgetEquipmentMenu.h"

#include "DarkKnightDebugHelper.h"
#include "Components/DkInventoryComponent.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Widgets/Inventory/DkInventoryDraggedItem.h"

void UDKWidgetEquipmentMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventoryComponent = UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer());
	// 绑定DraggedItem创建相关的回调
	InventoryComponent->OnDraggedItemCreated.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemCreated);
}

void UDKWidgetEquipmentMenu::HandleDraggedItemCreated(UDkInventoryDraggedItem* InDraggedItem)
{
	if (!IsValid(InDraggedItem)) return;
	
	if (IsValid(DraggedItem))
	{
		DraggedItem = nullptr;
	}
	DraggedItem = InDraggedItem;
	DraggedItem->OnDraggedItemClicked.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemClicked);
}

void UDKWidgetEquipmentMenu::HandleDraggedItemClicked(const FPointerEvent& MouseEvent)
{
	Debug::Print(TEXT("EquippedMenu的Clicked回调触发"));
}
