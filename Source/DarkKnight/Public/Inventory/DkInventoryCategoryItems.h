#pragma once
#include "DkInventoryItem.h"
#include "DkTypes/DkEnums.h"

#include "DkInventoryCategoryItems.generated.h"

USTRUCT()
struct FInventoryItemBriefInfo // 背包物品简约信息
{
	GENERATED_BODY()

	UPROPERTY()
	int Index = INDEX_NONE; // 类别里的第几个

	UPROPERTY()
	UDkInventoryItem* InventoryItem = nullptr;

	UPROPERTY()
	int StackCount = 0;

	bool IsEmpty() const { return InventoryItem == nullptr; }
	bool IsValid() const { return Index != INDEX_NONE; }
};

USTRUCT()
struct FInventoryCategoryItems
{
	GENERATED_BODY()

	UPROPERTY()
	EInventoryItemCategory Category;

	UPROPERTY()
	TArray<FInventoryItemBriefInfo> Items;

	void InitializeGrid(int32 Size);
	int32 FindEmptySlot() const;
	bool IsSlotEmpty(int32 Index) const;
	FInventoryItemBriefInfo* GetItemAt(int32 Index);
};

USTRUCT()
struct FInventoryCategoryItemsArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventoryCategoryItems> Array;
	
	bool ContainCategory(EInventoryItemCategory Category) const;
	TArray<FInventoryItemBriefInfo>* FindItems(EInventoryItemCategory Category);
	const TArray<FInventoryItemBriefInfo>* FindItems(EInventoryItemCategory Category) const;
	int GetCategoryMaxSize(EInventoryItemCategory Category) const;
	void AddNewCategory(EInventoryItemCategory Category, int Size);
	void AddItem(EInventoryItemCategory Category, const FInventoryItemBriefInfo& Item);
	void RemoveItem(EInventoryItemCategory Category, const FInventoryItemBriefInfo& Item);
	bool RemoveItemByIndex(EInventoryItemCategory Category, int32 Index, int32 RemoveCount = 1);
	bool RemoveItemByInventoryItem(
		EInventoryItemCategory Category, UDkInventoryItem* InventoryItem, int32 RemoveCount = 1
	);
};

USTRUCT(BlueprintType)
struct FInventoryItemCategoryGridSize // 背包物品各个类别的限制格子数量
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EInventoryItemCategory Category;

	UPROPERTY(EditAnywhere)
	int Rows = 0;

	UPROPERTY(EditAnywhere)
	int Columns = 0;

	// TODO: 后续这里还可以补个Icon，自动生成背包类型的按钮
};
