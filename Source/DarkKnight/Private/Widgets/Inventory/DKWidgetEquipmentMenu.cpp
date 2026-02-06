// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DKWidgetEquipmentMenu.h"

#include "DarkKnightDebugHelper.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/DkInventoryComponent.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Inventory/DkInventoryItem.h"
#include "Widgets/Inventory/DkInventoryDraggedItem.h"
#include "Widgets/Inventory/Equipment/DkInventoryEquipmentGridSlot.h"
#include "Widgets/Inventory/Equipment/DkInventoryEquipmentSlot.h"

void UDKWidgetEquipmentMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		UDkInventoryEquipmentGridSlot* EquippedGridSlot = Cast<UDkInventoryEquipmentGridSlot>(Widget);
		if (IsValid(EquippedGridSlot))
		{
			EquippedGridSlots.Add(EquippedGridSlot);
			EquippedGridSlot->GetEquipmentSlot()->GridSlotClicked.AddDynamic(this, &ThisClass::HandleEquipSlotClicked);
		}
	});

	InventoryComponent = UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer());
	// 绑定DraggedItem创建相关的回调
	InventoryComponent->OnDraggedItemCreated.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemCreated);
	InventoryComponent->OnDraggedItemRemoved.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemRemoved);
}

void UDKWidgetEquipmentMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (LastHighlightIndex != INDEX_NONE)
	{
		EquippedGridSlots[LastHighlightIndex]->SetOccupiedBrush();
	}

	// 根据鼠标的位置，更改Hover的格子样式
	const FVector2D CanvasPosition = UDkInventoryFunctionLibrary::GetWidgetPosition(EquipmentCanvasPanel);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	bMouseWithInCanvas = UDkInventoryFunctionLibrary::IsWithInBounds(
		CanvasPosition, UDkInventoryFunctionLibrary::GetWidgetSize(EquipmentCanvasPanel), MousePosition
	);
	if (!bMouseWithInCanvas && LastHighlightIndex != INDEX_NONE)
	{
		return;
	}

	CalculateHoveredSlot(CanvasPosition, MousePosition);
}

void UDKWidgetEquipmentMenu::CalculateHoveredSlot(const FVector2D& CanvasPosition, const FVector2D& MousePosition)
{
	// 如果鼠标不在GridCanvasPanel内，return
	if (!bMouseWithInCanvas) return;

	if (FMath::FloorToInt(MousePosition.Y - CanvasPosition.Y) %
		FMath::FloorToInt(EquippedGridSlots[0]->GetTotalSlotSize().Y) <
		FMath::FloorToInt(EquippedGridSlots[0]->GetTotalSlotSize().Y - EquippedGridSlots[0]->GetSlotSize().Y))
	{
		return;
	}

	// 计算网格的象限、索引、坐标
	const FIntPoint HoveredTileCoordinate = CalculateHoveredCoordinates(CanvasPosition, MousePosition);

	// Debug::Print(FString::Printf(TEXT("X索引：%d, Y索引：%d"), HoveredTileCoordinate.X, HoveredTileCoordinate.Y));

	// 改变Slot的样式
	if (!IsValid(DraggedItem)) return;

	ItemEquipIndex = HoveredTileCoordinate.X * NUM_OF_COLUMNS + HoveredTileCoordinate.Y;
	// Debug::Print(DraggedItem->GetItemTag().ToString());
	if (DraggedItem->GetItemTag().MatchesTag(EquippedGridSlots[ItemEquipIndex]->GetEquipmentTypeTag()))
	{
		EquippedGridSlots[ItemEquipIndex]->SetEnabledBrush();
	}
	else
	{
		EquippedGridSlots[ItemEquipIndex]->SetDisabledBrush();
	}
	LastHighlightIndex = ItemEquipIndex;
}

FIntPoint UDKWidgetEquipmentMenu::CalculateHoveredCoordinates(
	const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	return FIntPoint{
		static_cast<int32>(FMath::FloorToInt(
			(MousePosition.X - CanvasPosition.X) / EquippedGridSlots[0]->GetTotalSlotSize().X)),
		static_cast<int32>(FMath::FloorToInt(
			(MousePosition.Y - CanvasPosition.Y) / EquippedGridSlots[0]->GetTotalSlotSize().Y))
	};
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

void UDKWidgetEquipmentMenu::HandleDraggedItemRemoved()
{
	DraggedItem = nullptr;
}

void UDKWidgetEquipmentMenu::HandleDraggedItemClicked(const FPointerEvent& MouseEvent)
{
	if (!IsValid(DraggedItem)) return;
	if (!EquippedGridSlots.IsValidIndex(ItemEquipIndex)) return;
	if (!DraggedItem->GetItemTag().MatchesTag(EquippedGridSlots[ItemEquipIndex]->GetEquipmentTypeTag())) return;
	if (!bMouseWithInCanvas) return;

	// TODO: 如何已有Equipment，则交换俩者的位置
	// if (CurrentSpaceQueryResult.ValidItem.IsValid() && EquippedGridSlots.IsValidIndex(CurrentSpaceQueryResult.UpperLeftIndex))
	// {
	// 	OnSlottedItemClicked(CurrentSpaceQueryResult.UpperLeftIndex, MouseEvent);
	// 	return;
	// }

	UDkInventoryEquipmentGridSlot* EquipmentGridSlot = EquippedGridSlots[ItemEquipIndex];
	if (!IsValid(EquipmentGridSlot->GetInventoryItem()))
	{
		EquipmentGridSlot->UpdateEquipmentIcon(DraggedItem->GetInventoryItem());
	}

	check(InventoryComponent.IsValid());
	InventoryComponent->ServerUpdateEquippedItem(DraggedItem->GetInventoryItem(), nullptr);

	// 执行一些专属于Client的回调
	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(DraggedItem->GetInventoryItem());
	}
	
	ClearDraggedItem();
}

void UDKWidgetEquipmentMenu::ClearDraggedItem()
{
	if (!IsValid(DraggedItem)) return;

	DraggedItem->SetInventoryItem(nullptr);
	DraggedItem->SetIsStackable(false);
	DraggedItem->SetPreviousGridIndex(INDEX_NONE);
	DraggedItem->UpdateStackCount(0);
	DraggedItem->SetImageBrush(FSlateNoResource());

	DraggedItem->RemoveFromParent();
	DraggedItem = nullptr;
	check(InventoryComponent.IsValid());
	InventoryComponent->OnDraggedItemRemoved.Broadcast();
}

void UDKWidgetEquipmentMenu::HandleEquipSlotClicked(int GridIndex, const FPointerEvent& MouseEvent)
{
}
