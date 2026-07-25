// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkInventoryGridSlot.h"
#include "DkTypes/DkEnums.h"
#include "DkTypes/DkStructs.h"
#include "Inventory/DkInventoryCategoryItems.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventorySlotAvailabilty.h"
#include "DkInventoryItemGrid.generated.h"

struct FInventoryItemBriefInfo;
class UDkInventoryItemDescriptionMenu;
class UDkInventoryPopUpMenu;
struct FInventoryTileParameters;
struct FItemFragment_Grid;
struct FDkInventorySlotAvailabilityResult;
class UDkInventoryDraggedItem;
class UDkInventorySlottedItem;
class UDkInventoryItem;
class UDkItemComponent;
class UDkInventoryComponent;
class UUniformGridPanel;
class UDkInventoryGridSlot;
/**
 * 背包中每一类物品的网格整体
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryItemGrid : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	FDkInventorySlotAvailabilityResult HasRoomForItem(const UDkItemComponent* ItemComponent);
	FDkInventorySlotAvailabilityResult HasRoomForItem(const UDkInventoryItem* Item);

	void SetInventoryComp(UDkInventoryComponent* InInventoryComp);
	void ClearItems();

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~End UUserWidget Function

	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UUniformGridPanel> UniformGridPanel;
	//***** Bound Widgets *****//

	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;

	/* 添加/删除Item(部分方法弃用) */
	UFUNCTION()
	void AddItem(UDkInventoryItem* Item);

	UFUNCTION()
	void HandleStackChanged(const FDkInventorySlotAvailabilityResult& Result);

	virtual void AddStacks(const FDkInventorySlotAvailabilityResult& Result);

	void AddItemToIndices(const FDkInventorySlotAvailabilityResult& Result, UDkInventoryItem* NewItem);
	virtual void AddItemToIndex(UDkInventoryItem* NewItem, int32 Index, int32 StackAmount, bool bStackable);

	bool MatchesCategory(const UDkInventoryItem* Item) const;

	virtual FDkInventorySlotAvailabilityResult HasRoomForItem(const FInventoryItemManifest& Manifest);

	int32 CalculateFillAmountForSlot(
		const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UDkInventoryGridSlot* GridSlot
	) const;
	int32 GetStackAmount(const UDkInventoryGridSlot* GridSlot) const;

	virtual void PutDownOnIndex(const int32 Index);
	/********/

	/**********************************************************************/
	/*                             显示物品信息                             */
	/**********************************************************************/
public:
	void PopulateGrid(EInventoryItemCategory InCategory);

	void HandleInventorySlotArrayUpdated();
	
	void UpdateInventorySlotArray();
	
	/* 更新GridSlot背景 */
protected:
	void UpdateGridSlotInfo(const FInventoryItemBriefInfo& ItemBriefInfo, int Index, UDkInventoryGridSlot* GridSlot);
	virtual void UpdateGridSlots(UDkInventoryItem* NewItem, const int32 Index, int32 StackAmount, bool bStackable);
	/********/

	/* 构造Grid */
public:
	virtual void ConstructGrid(int32 InRows, int32 InColumns);

protected:
	UPROPERTY()
	TArray<TObjectPtr<UDkInventoryGridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventoryGridSlot> GridSlotClass;

	int32 Rows = 0;
	int32 Columns = 0;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize = 50.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float SlotDistance = 8.f;
	/********/

	/* 拖拽Item */
protected:
	bool IsSameStackableWithDraggedItem(const UDkInventoryItem* ClickedInventoryItem);

	virtual void SwapWithDraggedItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex);
	virtual void ConsumeDraggedItemStack(int32 ClickedStackCount, int32 DraggedStackCount, int32 GridIndex);

	void DragItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex);
	virtual void AssignDraggedItem(UDkInventoryItem* InventoryItem);
	virtual void AssignDraggedItem(
		UDkInventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex
	);
	virtual void RemoveItemFromGrid(UDkInventoryItem* InventoryItem, const int32 GridIndex);

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventoryDraggedItem> DraggedItemClass;

	UPROPERTY()
	TObjectPtr<UDkInventoryDraggedItem> DraggedItem;

	void ClearDraggedItem();

	UFUNCTION()
	void HandleDraggedItemCreated(UDkInventoryDraggedItem* InDraggedItem);

	UFUNCTION()
	void HandleDraggedItemRecovered(UDkInventoryDraggedItem* InDraggedItem);

	UFUNCTION()
	void HandleDraggedItemRemoved();

	UFUNCTION()
	void HandleDraggedItemClicked(const FPointerEvent& MouseEvent);

	virtual void OnDraggedItemClicked(const FPointerEvent& MouseEvent);

	// void RequestMoveItem(int MoveStackCount);
	// void RequestMoveItemFromEquipment(UDkInventoryItem* Item, int MoveStackCount);
	// void RequestSwapItem();
	/********/

	/* 鼠标Hover背包网格，改变其样式 */
	void UpdateItemDropIndex(const FVector2D& PanelPosition, const FVector2D& MousePosition);

	// 更改DraggedItem覆盖的背后区域的格子的样式
	void ChangeHoverType(const int32 InIndex, EInventoryGridSlotState GridSlotState);

	// 鼠标是否离开GridCanvasPanel对应的区域
	virtual bool CursorExitedCanvas(
		const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& MousePos
	);

	bool bMouseWithInCanvas;
	bool bLastMouseWithInCanvas;

	int32 ItemDropIndex = INDEX_NONE;

	int32 LastHighlightedIndex;

	// Highlight/UnHighlight DraggedItem覆盖的背后区域的格子
	void HighlightSlot(const int32 Index);
	void UnHighlightSlot(const int32 Index);

	// 没有DraggedItem时，更改鼠标覆盖区域的格子的样式
	UFUNCTION()
	void OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent);
	/********/

	/*
	 * 左键点击GridSlot
	 */
	UFUNCTION()
	void OnGridSlotClicked(int GridIndex, const FPointerEvent& MouseEvent);

	/* 右键菜单 */
	void CreateItemPopUp(const int32 GridIndex);

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UDkInventoryPopUpMenu> PopUpMenuClass;

	UPROPERTY()
	TObjectPtr<UDkInventoryPopUpMenu> PopUpMenu;

	// 拆解Item
	UFUNCTION()
	void HandlePopUpMenuSplit(int32 SplitAmount, int32 Index);

	virtual void OnPopUpMenuSplit(int32 SplitAmount, int32 Index);

	// 丢弃Item
	UFUNCTION()
	void OnPopUpMenuDrop(int32 Index);

	void DropItem();

	// 消耗Item
	UFUNCTION()
	void HandlePopUpMenuConsume(int32 Index);

	virtual void OnPopUpMenuConsume(int32 Index);
	/********/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	EInventoryItemCategory ItemCategory;

public:
	FORCEINLINE EInventoryItemCategory GetItemCategory() const { return ItemCategory; }
	void SetItemCategory(EInventoryItemCategory InCategory) { ItemCategory = InCategory; }
};
