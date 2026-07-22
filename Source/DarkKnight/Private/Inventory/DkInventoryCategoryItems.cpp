#include "Inventory/DkInventoryCategoryItems.h"

#include "DarkKnightDebugHelper.h"

void FInventoryCategoryItems::InitializeGrid(int32 Size)
{
	Items.SetNum(Size);
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		Items[i].Index = i;
		Items[i].InventoryItem = nullptr;
		Items[i].StackCount = 0;
	}
}

int32 FInventoryCategoryItems::FindEmptySlot() const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].InventoryItem == nullptr)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

bool FInventoryCategoryItems::IsSlotEmpty(int32 Index) const
{
	return Items.IsValidIndex(Index) && Items[Index].InventoryItem == nullptr;
}

FInventoryItemBriefInfo* FInventoryCategoryItems::GetItemAt(int32 Index)
{
	return Items.IsValidIndex(Index) ? &Items[Index] : nullptr;
}

bool FInventoryCategoryItemsArray::ContainCategory(EInventoryItemCategory Category) const
{
	for (const FInventoryCategoryItems& CategoryItems : Array)
	{
		if (CategoryItems.Category == Category)
		{
			return true;
		}
	}
	return false;
}

TArray<FInventoryItemBriefInfo>* FInventoryCategoryItemsArray::FindItems(EInventoryItemCategory Category)
{
	for (FInventoryCategoryItems& CategoryItems : Array)
	{
		if (CategoryItems.Category == Category)
		{
			return &CategoryItems.Items;
		}
	}
	return nullptr;
}

const TArray<FInventoryItemBriefInfo>* FInventoryCategoryItemsArray::FindItems(EInventoryItemCategory Category) const
{
	return const_cast<FInventoryCategoryItemsArray*>(this)->FindItems(Category);
}

int FInventoryCategoryItemsArray::GetCategoryMaxSize(EInventoryItemCategory Category) const
{
	const TArray<FInventoryItemBriefInfo>* Items = FindItems(Category);
	return Items ? Items->Num() : 0;
}

void FInventoryCategoryItemsArray::AddNewCategory(EInventoryItemCategory Category, int Size)
{
	for (FInventoryCategoryItems& CategoryItems : Array)
	{
		if (CategoryItems.Category == Category)
		{
			Debug::Print("FInventoryCategoryItemsArray在添加Category时，已存在对应Category");
			return;
		}
	}

	FInventoryCategoryItems NewCategory;
	NewCategory.Category = Category;
	NewCategory.InitializeGrid(Size);
	Array.Add(MoveTemp(NewCategory));
}

void FInventoryCategoryItemsArray::AddItem(EInventoryItemCategory Category, const FInventoryItemBriefInfo& Item)
{
	// 尝试找到已有 Category
	for (FInventoryCategoryItems& CategoryItems : Array)
	{
		if (CategoryItems.Category == Category)
		{
			// 检查是否可堆叠（假设同一 InventoryItem 可以堆叠）
			if (Item.InventoryItem != nullptr)
			{
				for (FInventoryItemBriefInfo& ExistingItem : CategoryItems.Items)
				{
					if (ExistingItem.InventoryItem == Item.InventoryItem)
					{
						// 同一物品，增加堆叠数
						ExistingItem.StackCount += Item.StackCount;
						return;
					}
				}
			}
			// 不可堆叠或新物品类型，直接添加
			CategoryItems.Items.Add(Item);
			return;
		}
	}

	// 没有找到对应 Category，新建一个
	FInventoryCategoryItems NewCategory;
	NewCategory.Category = Category;
	NewCategory.Items.Add(Item);
	Array.Add(NewCategory);
}

void FInventoryCategoryItemsArray::RemoveItem(EInventoryItemCategory Category, const FInventoryItemBriefInfo& Item)
{
	for (int32 i = 0; i < Array.Num(); ++i)
	{
		if (Array[i].Category == Category)
		{
			TArray<FInventoryItemBriefInfo>& Items = Array[i].Items;

			// 优先按 Index 匹配
			if (Item.Index >= 0)
			{
				for (int32 j = 0; j < Items.Num(); ++j)
				{
					if (Items[j].Index == Item.Index)
					{
						// 减少堆叠数，如果减完还有剩余则更新，否则移除
						if (Items[j].StackCount > Item.StackCount)
						{
							Items[j].StackCount -= Item.StackCount;
						}
						else
						{
							Items.RemoveAtSwap(j);
						}
						break;
					}
				}
			}
			// 否则按 InventoryItem 指针匹配
			else if (Item.InventoryItem != nullptr)
			{
				for (int32 j = 0; j < Items.Num(); ++j)
				{
					if (Items[j].InventoryItem == Item.InventoryItem)
					{
						if (Items[j].StackCount > Item.StackCount)
						{
							Items[j].StackCount -= Item.StackCount;
						}
						else
						{
							Items.RemoveAtSwap(j);
						}
						break;
					}
				}
			}

			// 如果该 Category 下没有物品了，可选：移除整个 Category
			// if (Items.Num() == 0)
			// {
			//     Array.RemoveAtSwap(i);
			// }
			return;
		}
	}
}

bool FInventoryCategoryItemsArray::RemoveItemByIndex(EInventoryItemCategory Category, int32 Index, int32 RemoveCount)
{
	for (int32 i = 0; i < Array.Num(); ++i)
	{
		if (Array[i].Category == Category)
		{
			TArray<FInventoryItemBriefInfo>& Items = Array[i].Items;
			for (int32 j = 0; j < Items.Num(); ++j)
			{
				if (Items[j].Index == Index)
				{
					if (Items[j].StackCount > RemoveCount)
					{
						Items[j].StackCount -= RemoveCount;
					}
					else
					{
						Items.RemoveAtSwap(j);
					}
					return true;
				}
			}
			break;
		}
	}
	return false;
}

bool FInventoryCategoryItemsArray::RemoveItemByInventoryItem(
	EInventoryItemCategory Category, UDkInventoryItem* InventoryItem, int32 RemoveCount)
{
	if (InventoryItem == nullptr) return false;

	for (int32 i = 0; i < Array.Num(); ++i)
	{
		if (Array[i].Category == Category)
		{
			TArray<FInventoryItemBriefInfo>& Items = Array[i].Items;
			for (int32 j = 0; j < Items.Num(); ++j)
			{
				if (Items[j].InventoryItem == InventoryItem)
				{
					if (Items[j].StackCount > RemoveCount)
					{
						Items[j].StackCount -= RemoveCount;
					}
					else
					{
						Items.RemoveAtSwap(j);
					}
					return true;
				}
			}
			break;
		}
	}
	return false;
}
