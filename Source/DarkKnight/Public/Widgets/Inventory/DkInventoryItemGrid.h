// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkInventoryGridSlot.h"
#include "DkTypes/DkEnums.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventorySlotAvailabilty.h"
#include "DkInventoryItemGrid.generated.h"

class UDkInventoryDraggedItem;
struct FInventoryItemGridFragment;
class UDkInventorySlottedItem;
struct FDkInventorySlotAvailabilityResult;
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
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	bool IsRightMouseClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftMouseClick(const FPointerEvent& MouseEvent) const;

	void DragItem(UDkInventoryItem* ClickedInventoryItem, const int32 GridIndex);
	virtual void AssignDraggedItem(UDkInventoryItem* InventoryItem);
	virtual void AssignDraggedItem(
		UDkInventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex
	);

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventoryDraggedItem> DraggedItemClass;

	UPROPERTY()
	TObjectPtr<UDkInventoryDraggedItem> DraggedItem;
	/********/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	EInventoryItemCategory ItemCategory;

public:
	FORCEINLINE EInventoryItemCategory GetItemCategory() const { return ItemCategory; }
};
