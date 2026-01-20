// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryItemGrid.h"
#include "DKInventoryItemSpatialGrid.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDKInventoryItemSpatialGrid : public UDkInventoryItemGrid
{
	GENERATED_BODY()

protected:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> GridCanvasPanel;
	//***** Bound Widgets *****//

	/* 添加/删除Item */
	virtual void AddItemToIndex(UDkInventoryItem* NewItem, int32 Index, int32 StackAmount, bool bStackable) override;
	FVector2D GetDrawSize(const FInventoryItemGridFragment* GridFragment) const;
	void AddSlottedItemToCanvas(
		const int32 Index, const FInventoryItemGridFragment* GridFragment, UDkInventorySlottedItem* SlottedItem
	) const;

	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimension) const;

	int32 CalculateFillAmountForSlot(
		const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UDkInventoryGridSlot* GridSlot
	) const;
	int32 GetStackAmount(const UDkInventoryGridSlot* GridSlot) const;

	virtual FDkInventorySlotAvailabilityResult HasRoomForItem(const FInventoryItemManifest& Manifest) override;
	/********/

	/* 更新GridSlot背景 */
	virtual void UpdateGridSlots(
		UDkInventoryItem* NewItem, const int32 Index, int32 StackAmount, bool bStackable
	) override;
	/********/

	/* 构造Grid */
	virtual void ConstructGrid() override;
	/********/

	/* 拖拽Item */
	virtual void AssignDraggedItem(UDkInventoryItem* InventoryItem) override;
	/********/
};
