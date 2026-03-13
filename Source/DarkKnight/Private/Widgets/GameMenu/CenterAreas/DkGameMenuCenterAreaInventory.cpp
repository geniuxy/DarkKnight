// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/CenterAreas/DkGameMenuCenterAreaInventory.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/DkInventoryComponent.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Widgets/Inventory/DkInventoryDraggedItem.h"

void UDkGameMenuCenterAreaInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventoryComponent = UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer());
	// 绑定DraggedItem创建相关的回调
	InventoryComponent->OnDraggedItemCreated.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemCreated);
	InventoryComponent->OnDraggedItemRemoved.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemRemoved);
	// 绑定ItemDescriptionMenu创建相关的回调
	InventoryComponent->OnItemDescriptionMenuCreated.AddUniqueDynamic(this, &ThisClass::HandleItemDescriptionCreated);
	InventoryComponent->OnItemDescriptionMenuRemoved.AddUniqueDynamic(this, &ThisClass::HandleItemDescriptionRemoved);
}

void UDkGameMenuCenterAreaInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 拖拽Item，跟随鼠标移动
	if (DraggedItem.IsValid() && DraggedItem->IsInViewport())
	{
		// 全程用硬件像素，以将鼠标定位到DraggedItem中心
		FVector2D MousePos;
		if (UGameViewportClient* VP = GetWorld()->GetGameViewport())
		{
			VP->GetMousePosition(MousePos); // 系统硬件像素（受 DPI 缩放）
		}

		const FVector2D ActualSize = DraggedItem->GetCachedGeometry().GetAbsoluteSize();
		DraggedItem->SetPositionInViewport(MousePos - ActualSize * 0.5f);
	}

	if (ItemDescriptionMenu.IsValid())
	{
		UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemDescriptionMenu.Get());
		if (!IsValid(ItemDescriptionCPS)) return;

		const FVector2D ItemDescriptionSize = ItemDescriptionMenu->GetBoxSize();
		ItemDescriptionCPS->SetSize(ItemDescriptionSize);

		FVector2D CanvasPanelPosition = UDkUIFunctionLibrary::GetWidgetPosition(CanvasPanel);
		FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
		FVector2D RelativePosition = UDkUIFunctionLibrary::GetRelativeWidgetPosition(
			CanvasPanelPosition,
			UDkUIFunctionLibrary::GetWidgetSize(this),
			ItemDescriptionSize,
			MousePos
		);
		ItemDescriptionCPS->SetPosition(RelativePosition);
	}
}

void UDkGameMenuCenterAreaInventory::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);

	if (DraggedItem.IsValid() && DraggedItem->IsInViewport())
	{
		check(InventoryComponent.IsValid());
		if (DraggedItem->IsPreviousEquipped())
		{
			InventoryComponent->OnExitGameMenuRecoverEquippedItem.Broadcast(DraggedItem.Get());
		}
		else
		{
			InventoryComponent->OnExitGameMenuRecoverGridItem.Broadcast(DraggedItem.Get());
		}
		if (DraggedItem.IsValid())
		{
			DraggedItem->RemoveFromParent();
		}
		DraggedItem = nullptr;
		InventoryComponent->OnDraggedItemRemoved.Broadcast();
	}
}

void UDkGameMenuCenterAreaInventory::HandleDraggedItemCreated(UDkInventoryDraggedItem* InDraggedItem)
{
	if (!IsValid(InDraggedItem)) return;

	if (DraggedItem.IsValid())
	{
		DraggedItem = nullptr;
	}
	DraggedItem = InDraggedItem;
}

void UDkGameMenuCenterAreaInventory::HandleDraggedItemRemoved()
{
	DraggedItem = nullptr;
}

void UDkGameMenuCenterAreaInventory::HandleItemDescriptionCreated(
	UDkInventoryItemDescriptionMenu* InItemDescriptionMenu)
{
	if (!IsValid(InItemDescriptionMenu)) return;

	if (ItemDescriptionMenu.IsValid())
	{
		ItemDescriptionMenu = nullptr;
	}
	ItemDescriptionMenu = InItemDescriptionMenu;
	CanvasPanel->AddChild(ItemDescriptionMenu.Get());
}

void UDkGameMenuCenterAreaInventory::HandleItemDescriptionRemoved()
{
	CanvasPanel->RemoveChild(ItemDescriptionMenu.Get());
	ItemDescriptionMenu = nullptr;
}
