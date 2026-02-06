// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DKWidgetEquipmentMenu.h"

#include "DarkKnightDebugHelper.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/DkInventoryComponent.h"
#include "FunctionLibrarys/DkCommonFunctionLibrary.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"
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
			EquippedGridSlot->GetEquipmentSlot()->SetTileIndex(EquippedGridSlots.Num() - 1);
			if (!IsValid(EquippedGridSlot->GetInventoryItem()))
			{
				EquippedGridSlot->UpdateEquipmentInfo(EquippedGridSlot->GetInventoryItem());
			}
			EquippedGridSlot->GetEquipmentSlot()->GridSlotClicked.AddDynamic(this, &ThisClass::HandleEquipSlotClicked);
		}
	});

	InventoryComponent = UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer());
	// 绑定DraggedItem创建相关的回调
	InventoryComponent->OnDraggedItemCreated.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemCreated);
	InventoryComponent->OnDraggedItemRemoved.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemRemoved);
	InventoryComponent->OnExitGameMenuRecoverEquippedItem.AddUniqueDynamic(
		this, &ThisClass::HandleDraggedItemRecovered
	);
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

void UDKWidgetEquipmentMenu::PutItemOnEquipSlot(int32 EquipIndex)
{
	UDkInventoryEquipmentGridSlot* EquipmentGridSlot = EquippedGridSlots[EquipIndex];
	if (!IsValid(EquipmentGridSlot->GetInventoryItem()))
	{
		EquipmentGridSlot->UpdateEquipmentInfo(DraggedItem->GetInventoryItem());
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

void UDKWidgetEquipmentMenu::HandleDraggedItemRecovered(UDkInventoryDraggedItem* InDraggedItem)
{
	if (InDraggedItem->IsPreviousEquipped())
	{
		if (IsValid(InDraggedItem->GetInventoryItem()))
		{
			PutItemOnEquipSlot(InDraggedItem->GetPreviousGridIndex());
		}
	}
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

	PutItemOnEquipSlot(ItemEquipIndex);
}

void UDKWidgetEquipmentMenu::ClearDraggedItem()
{
	if (!IsValid(DraggedItem)) return;

	DraggedItem->SetInventoryItem(nullptr);
	DraggedItem->SetIsStackable(false);
	DraggedItem->SetPreviousGridIndex(INDEX_NONE);
	DraggedItem->UpdateStackCount(0);
	DraggedItem->SetImageBrush(FSlateNoResource());

	check(InventoryComponent.IsValid());
	if (IsValid(DraggedItem))
	{
		DraggedItem->RemoveFromParent();
	}
	DraggedItem = nullptr;
	InventoryComponent->OnDraggedItemRemoved.Broadcast();
}

void UDKWidgetEquipmentMenu::DragItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	AssignDraggedItem(ClickedInventoryItem, GridIndex, GridIndex);

	// 从背包中移除被点击的Item
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
}

void UDKWidgetEquipmentMenu::AssignDraggedItem(UDkInventoryItem* InventoryItem)
{
	if (!IsValid(DraggedItem))
	{
		DraggedItem = CreateWidget<UDkInventoryDraggedItem>(GetOwningPlayer(), DraggedItemClass);
	}

	const FInventoryItemGridFragment* GridFragment =
		GetFragment<FInventoryItemGridFragment>(InventoryItem, DkGameplayTags::Dk_Inventory_Fragment_Grid);
	const FInventoryItemImageFragment* ImageFragment =
		GetFragment<FInventoryItemImageFragment>(InventoryItem, DkGameplayTags::Dk_Inventory_Fragment_Icon);
	if (!GridFragment || !ImageFragment) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = EquippedGridSlots[0]->GetEquipmentSlot()->GetIconSize();

	DraggedItem->SetImageBrush(Brush);
	DraggedItem->SetGridDimension(GridFragment->GetGridSize());
	DraggedItem->SetInventoryItem(InventoryItem);
	DraggedItem->SetIsStackable(InventoryItem->IsItemStackable());
	check(InventoryComponent.IsValid());
	InventoryComponent->OnDraggedItemCreated.Broadcast(DraggedItem);
	DraggedItem->OnDraggedItemClicked.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemClicked);

	DraggedItem->SetDesiredSizeInViewport(Brush.ImageSize);
	DraggedItem->AddToViewport();
}

void UDKWidgetEquipmentMenu::AssignDraggedItem(
	UDkInventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex)
{
	AssignDraggedItem(InventoryItem);

	DraggedItem->SetPreviousGridIndex(PreviousGridIndex);
	DraggedItem->SetIsPreviousEquipped(true);
	// 默认装备的Count为1且不可叠加
	DraggedItem->UpdateStackCount(0);
}

void UDKWidgetEquipmentMenu::RemoveItemFromGrid(UDkInventoryItem* InventoryItem, const int32 GridIndex)
{
	if (!EquippedGridSlots.IsValidIndex(GridIndex)) return;
	EquippedGridSlots[GridIndex]->GetEquipmentSlot()->SetInventoryItem(nullptr);
	EquippedGridSlots[GridIndex]->GetEquipmentSlot()->SetUnoccupiedTexture();
	EquippedGridSlots[GridIndex]->GetEquipmentSlot()->SetIsAvailable(true);
	EquippedGridSlots[GridIndex]->GetEquipmentSlot()->SetStackCount(0);
	EquippedGridSlots[GridIndex]->SetDefaultBackGroundIcon();
}

void UDKWidgetEquipmentMenu::HandleEquipSlotClicked(int GridIndex, const FPointerEvent& MouseEvent)
{
	check(EquippedGridSlots.IsValidIndex(GridIndex));
	UDkInventoryItem* ClickedInventoryItem = EquippedGridSlots[GridIndex]->GetInventoryItem();
	if (!IsValid(DraggedItem) && UDkCommonFunctionLibrary::IsLeftMouseClick(MouseEvent))
	{
		// 拖拽Item
		DragItem(ClickedInventoryItem, GridIndex);
		return;
	}

	// if (UDkCommonFunctionLibrary::IsRightMouseClick(MouseEvent))
	// {
	// 	CreateItemPopUp(GridIndex);
	// 	return;
	// }
	//
	// // DraggedItem和被点击的Item是一个类型吗，他们都可堆叠吗？
	// if (IsSameStackableWithDraggedItem(ClickedInventoryItem))
	// {
	// 	const int32 ClickedStackCount = GridSlots[GridIndex]->GetStackCount();
	// 	const FInventoryItemStackableFragment* StackableFragment =
	// 		ClickedInventoryItem->GetItemManifest().GetFragmentOfType<FInventoryItemStackableFragment>();
	// 	const int32 MaxStackSize = StackableFragment->GetMaxStackSize();
	// 	const int32 SpaceInClickedSlot = MaxStackSize - ClickedStackCount;
	// 	const int32 DraggedStackCount = DraggedItem->GetStackCount();
	// 	// 是否应该交换SlottedItem和DraggedItem
	// 	if (SpaceInClickedSlot == 0 && DraggedStackCount < MaxStackSize)
	// 	{
	// 		// 交换SlottedItem和DraggedItem
	// 		UDkInventoryGridSlot* GridSlot = GridSlots[GridIndex];
	// 		GridSlot->SetStackCount(DraggedStackCount);
	//
	// 		UDkInventorySlottedItem* SlottedItem = SlottedItemMap.FindChecked(GridIndex);
	// 		SlottedItem->UpdateStackCount(DraggedStackCount);
	//
	// 		DraggedItem->UpdateStackCount(ClickedStackCount);
	//
	// 		return;
	// 	}
	//
	// 	//	是否可以合并DraggedItem
	// 	if (SpaceInClickedSlot >= DraggedStackCount)
	// 	{
	// 		ConsumeDraggedItemStack(ClickedStackCount, DraggedStackCount, GridIndex);
	// 		return;
	// 	}
	//
	// 	//	是否可以填充ClickedItem,并更新DraggedItem
	// 	if (SpaceInClickedSlot < DraggedStackCount)
	// 	{
	// 		// 填充ClickedItem,并更新DraggedItem
	// 		UDkInventoryGridSlot* GridSlot = GridSlots[GridIndex];
	// 		GridSlot->SetStackCount(MaxStackSize);
	//
	// 		UDkInventorySlottedItem* SlottedItem = SlottedItemMap.FindChecked(GridIndex);
	// 		SlottedItem->UpdateStackCount(MaxStackSize);
	//
	// 		DraggedItem->UpdateStackCount(DraggedStackCount - SpaceInClickedSlot);
	//
	// 		return;
	// 	}
	//
	// 	return;
	// }
	// // 和DraggedItem交换位置
	// SwapWithDraggedItem(ClickedInventoryItem, GridIndex);
}
