// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryItemGrid.h"
#include "DKInventoryItemSpatialGrid.generated.h"

class UCanvasPanel;
/**
 * 背包中每一类物品的网格整体(支持物品图标不同大小)
 */
UCLASS()
class DARKKNIGHT_API UDKInventoryItemSpatialGrid : public UDkInventoryItemGrid
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget Function
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~End UUserWidget Function

	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> GridCanvasPanel;
	//***** Bound Widgets *****//

	/* 添加/删除Item */
	virtual void AddStacks(const FDkInventorySlotAvailabilityResult& Result) override;

	virtual void AddItemToIndex(UDkInventoryItem* NewItem, int32 Index, int32 StackAmount, bool bStackable) override;
	FVector2D GetDrawSize(const FInventoryItemGridFragment* GridFragment) const;
	void AddSlottedItemToCanvas(
		const int32 Index, const FInventoryItemGridFragment* GridFragment, UDkInventorySlottedItem* SlottedItem
	) const;

	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimension) const;

	bool CheckSlotConstraints(
		const UDkInventoryGridSlot* CurIndexGridSlot,
		const UDkInventoryGridSlot* SubGridSlot,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTemporarilyClaimed,
		const FGameplayTag& ItemTag,
		const int32 MaxStackCount
	) const;

	bool HasRoomAtIndex(
		const UDkInventoryGridSlot* CurIndexGridSlot,
		const FIntPoint& Dimension,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTemporarilyClaimed,
		const FGameplayTag& ItemTag,
		const int32 MaxStackCount
	);

	virtual FDkInventorySlotAvailabilityResult HasRoomForItem(const FInventoryItemManifest& Manifest) override;

	virtual void PutDownOnIndex(const int32 Index) override;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventorySlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UDkInventorySlottedItem>> SlottedItemMap;
	/********/

	/* 更新GridSlot背景 */
	virtual void UpdateGridSlots(
		UDkInventoryItem* NewItem, const int32 Index, int32 StackAmount, bool bStackable
	) override;
	/********/

	/* 构造Grid */
	virtual void ConstructGrid() override;

	virtual void RemoveItemFromGrid(UDkInventoryItem* InventoryItem, const int32 GridIndex) override;
	/********/

	/* 拖拽Item */
	UFUNCTION()
	void HandleSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	virtual void SwapWithDraggedItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex) override;

	virtual void ConsumeDraggedItemStack(int32 ClickedStackCount, int32 DraggedStackCount, int32 GridIndex) override;

	virtual void AssignDraggedItem(UDkInventoryItem* InventoryItem) override;

	virtual void AssignDraggedItem(
		UDkInventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex
	) override;

	virtual void OnDraggedItemClicked(const FPointerEvent& MouseEvent) override;
	/********/

	/* 鼠标Hover背包网格，改变其样式 */
	void UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition);
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	EInventoryTileQuadrant CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	void OnTileParametersUpdated(const FInventoryTileParameters& Parameters);

	FInventoryTileParameters TileParameters; // 鼠标所处格子的相关数据(索引、坐标、象限)
	FInventoryTileParameters LastTileParameters;

	FIntPoint CalculateStartingCoordinate(
		const FIntPoint& Coordinate, const FIntPoint& Dimension, const EInventoryTileQuadrant Quadrant
	) const;

	// 查看DraggedItem覆盖的背后区域的格子(是否有别的Item)
	FInventorySpaceQueryResult CheckHoverPosition(const FIntPoint& Dimension);
	FInventorySpaceQueryResult CurrentSpaceQueryResult;

	// 更改DraggedItem覆盖的背后区域的格子的样式
	void ChangeHoverType(const int32 StartingIndex, const FIntPoint& Dimension, EInventoryGridSlotState GridSlotState);

	// 鼠标是否离开GridCanvasPanel对应的区域
	virtual bool CursorExitedCanvas(
		const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& MousePos
	) override;

	// Highlight/UnHighlight DraggedItem覆盖的背后区域的格子
	void HighlightSlots(const int32 Index, const FIntPoint& Dimension);
	void UnHighlightSlots(const int32 Index, const FIntPoint& Dimension);

	FIntPoint LastHighlightedDimension;
	/********/

	/* 右键菜单 */
	virtual void OnPopUpMenuSplit(int32 SplitAmount, int32 Index) override;

	virtual void OnPopUpMenuConsume(int32 Index) override;
	/********/
};
