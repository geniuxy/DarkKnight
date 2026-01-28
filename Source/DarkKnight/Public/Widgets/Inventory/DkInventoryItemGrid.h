// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkInventoryGridSlot.h"
#include "DkTypes/DkEnums.h"
#include "DkTypes/DkStructs.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventorySlotAvailabilty.h"
#include "DkInventoryItemGrid.generated.h"

class UDkInventoryPopUpMenu;
struct FInventoryTileParameters;
struct FInventoryItemGridFragment;
struct FDkInventorySlotAvailabilityResult;
class UDkInventoryDraggedItem;
class UDkInventorySlottedItem;
class UDkInventoryItem;
class UDkItemComponent;
class UDkInventoryComponent;
class UUniformGridPanel;
class UDkInventoryGridSlot;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryItemGrid : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	FDkInventorySlotAvailabilityResult HasRoomForItem(const UDkItemComponent* ItemComponent);

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;
	//~End UUserWidget Function

	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UUniformGridPanel> GridPanel;
	//***** Bound Widgets *****//

	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;

	/* 添加/删除Item */
	UFUNCTION()
	void AddItem(UDkInventoryItem* Item);

	UFUNCTION()
	void AddStacks(const FDkInventorySlotAvailabilityResult& Result);

	void AddItemToIndices(const FDkInventorySlotAvailabilityResult& Result, UDkInventoryItem* NewItem);
	virtual void AddItemToIndex(UDkInventoryItem* NewItem, int32 Index, int32 StackAmount, bool bStackable);

	bool HasRoomAtIndex(
		const UDkInventoryGridSlot* CurIndexGridSlot,
		const FIntPoint& Dimension,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTemporarilyClaimed,
		const FGameplayTag& ItemTag,
		const int32 MaxStackCount
	);
	bool CheckSlotConstraints(
		const UDkInventoryGridSlot* CurIndexGridSlot,
		const UDkInventoryGridSlot* SubGridSlot,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTemporarilyClaimed,
		const FGameplayTag& ItemTag,
		const int32 MaxStackCount
	) const;
	bool MatchesCategory(const UDkInventoryItem* Item) const;

	FDkInventorySlotAvailabilityResult HasRoomForItem(const UDkInventoryItem* Item);
	virtual FDkInventorySlotAvailabilityResult HasRoomForItem(const FInventoryItemManifest& Manifest);

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventorySlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UDkInventorySlottedItem>> SlottedItemMap;
	/********/

	/* 更新GridSlot背景 */
	virtual void UpdateGridSlots(UDkInventoryItem* NewItem, const int32 Index, int32 StackAmount, bool bStackable);
	/********/

	/* 构造Grid */
	virtual void ConstructGrid();

	UPROPERTY()
	TArray<TObjectPtr<UDkInventoryGridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventoryGridSlot> GridSlotClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows = 8;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns = 8;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize = 50.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float SlotDistance = 8.f;
	/********/

	/* 拖拽Item */
	UFUNCTION()
	void HandleSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	virtual void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	bool IsSameStackableWithDraggedItem(const UDkInventoryItem* ClickedInventoryItem);

	virtual void SwapWithDraggedItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex);
	virtual void ConsumeDraggedItemStack(int32 ClickedStackCount, int32 DraggedStackCount, int32 GridIndex);

	bool IsRightMouseClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftMouseClick(const FPointerEvent& MouseEvent) const;

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
	/********/

	/* 鼠标Hover背包网格，改变其样式 */
	FInventoryTileParameters TileParameters; // 鼠标所处格子的相关数据(索引、坐标、象限)
	FInventoryTileParameters LastTileParameters;
	
	void ClearDraggedItem();

	UFUNCTION()
	void OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent);
	/********/

	/* 右键菜单 */
	virtual void CreateItemPopUp(const int32 GridIndex);

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UDkInventoryPopUpMenu> PopUpMenuClass;

	UPROPERTY()
	TObjectPtr<UDkInventoryPopUpMenu> PopUpMenu;

	UFUNCTION()
	void OnPopUpMenuSplit(int32 SplitAmount, int32 Index);

	// 丢弃Item
	UFUNCTION()
	void OnPopUpMenuDrop(int32 Index);

	void DropItem();

	// 消耗Item
	UFUNCTION()
	void OnPopUpMenuConsume(int32 Index);
	/********/
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	EInventoryItemCategory ItemCategory;

public:
	FORCEINLINE EInventoryItemCategory GetItemCategory() const { return ItemCategory; }
};
