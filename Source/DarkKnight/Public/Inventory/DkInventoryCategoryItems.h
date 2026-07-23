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
struct FInventoryItemCategoryInfo // 背包物品各个类别的相关信息
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EInventoryItemCategory Category;
	
	UPROPERTY(EditAnywhere)
	UTexture2D* CategoryIcon = nullptr;
	
	UPROPERTY(EditAnywhere)
	int Rows = 0;

	UPROPERTY(EditAnywhere)
	int Columns = 0;
};

// 这个类的作用是因为ListView的AddItem只接受UObject，AI推荐我创建个UObject来包装
UCLASS()
class DARKKNIGHT_API UInventoryItemCategoryData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FInventoryItemCategoryInfo Info;
};