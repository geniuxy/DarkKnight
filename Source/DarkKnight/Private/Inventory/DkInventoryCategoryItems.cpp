#include "Inventory/DkInventoryCategoryItems.h"

#include "Components/InventoryComps/DkInventoryComponent.h"


void FDkInventorySlotEntry::PreReplicatedRemove(const FFastArraySerializer& InArraySerializer)
{
}

void FDkInventorySlotEntry::PostReplicatedAdd(const FFastArraySerializer& InArraySerializer)
{
	const FDkInventorySlotArray& Array = static_cast<const FDkInventorySlotArray&>(InArraySerializer);
	if (UDkInventoryComponent* InventoryComponent = Cast<UDkInventoryComponent>(Array.OwnerComp))
	{
		InventoryComponent->OnInventorySlotArrayUpdated.Broadcast();
	}
}

void FDkInventorySlotEntry::PostReplicatedChange(const FFastArraySerializer& InArraySerializer)
{
	const FDkInventorySlotArray& Array = static_cast<const FDkInventorySlotArray&>(InArraySerializer);
	if (UDkInventoryComponent* InventoryComponent = Cast<UDkInventoryComponent>(Array.OwnerComp))
	{
		InventoryComponent->OnInventorySlotArrayUpdated.Broadcast();
	}
}

bool FDkInventorySlotEntry::IsSameItemByTag(FGameplayTag InTag) const
{
	return BriefInfo.InventoryItem->GetItemTag() == InTag; 
}

FDkInventorySlotEntry* FDkInventorySlotArray::FindByGlobalIndex(int32 InGlobalIndex)
{
	if (InGlobalIndex == INDEX_NONE) return nullptr;
	return Slots.FindByPredicate([InGlobalIndex](const FDkInventorySlotEntry& Entry)
		{
			return Entry.GlobalIndex == InGlobalIndex;
		}
	);
}

FDkInventorySlotEntry* FDkInventorySlotArray::FindBySlotIndex(EInventoryItemCategory InCategory, int32 InSlotIndex)
{
	if (InSlotIndex == INDEX_NONE) return nullptr;
	return Slots.FindByPredicate([=](const FDkInventorySlotEntry& Entry)
		{
			return Entry.Category == InCategory && Entry.BriefInfo.Index == InSlotIndex;
		}
	);
}

TArray<FDkInventorySlotEntry*> FDkInventorySlotArray::FindByCategory(EInventoryItemCategory InCategory)
{
	if (InCategory == EInventoryItemCategory::None) return TArray<FDkInventorySlotEntry*>();
	TArray<FDkInventorySlotEntry*> Result;
	for (FDkInventorySlotEntry& Slot : Slots)
	{
		if (Slot.Category == InCategory)
		{
			Result.Add(&Slot);
		}
	}
	return Result;
}
