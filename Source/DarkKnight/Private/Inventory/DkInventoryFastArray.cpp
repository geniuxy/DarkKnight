#include "Inventory/DkInventoryFastArray.h"

#include "Components/DkInventoryComponent.h"
#include "Components/DkItemComponent.h"
#include "Inventory/DkInventoryItem.h"


TArray<UDkInventoryItem*> FDkInventoryFastArray::GetAllItems() const
{
	TArray<UDkInventoryItem*> Results;
	Results.Reserve(Entries.Num());
	for (const auto& Entry : Entries)
	{
		if (!IsValid(Entry.Item)) continue;
		Results.Add(Entry.Item);
	}
	return Results;
}

void FDkInventoryFastArray::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	UDkInventoryComponent* InventoryComponent = Cast<UDkInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : RemovedIndices)
	{
		InventoryComponent->OnItemRemoved.Broadcast(Entries[Index].Item);
	}
}

void FDkInventoryFastArray::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	UDkInventoryComponent* InventoryComponent = Cast<UDkInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : AddedIndices)
	{
		InventoryComponent->OnItemAdded.Broadcast(Entries[Index].Item);
		InventoryComponent->OnAddItemNotice.Broadcast(
			Entries[Index].Item->GetItemName(),
			Entries[Index].Item->IsItemStackable() ? Entries[Index].Item->GetTotalStackCount() : 1 
		);
	}
}

UDkInventoryItem* FDkInventoryFastArray::AddEntry(UDkItemComponent* ItemComponent)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	UDkInventoryComponent* InventoryComponent = Cast<UDkInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return nullptr;

	FDkInventoryFastArrayEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = ItemComponent->GetItemManifest().Manifest(OwningActor);

	InventoryComponent->AddRepSubObj(NewEntry.Item);
	MarkItemDirty(NewEntry);
	return NewEntry.Item;
}

UDkInventoryItem* FDkInventoryFastArray::AddEntry(UDkInventoryItem* Item)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	UDkInventoryComponent* InventoryComponent = Cast<UDkInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return nullptr;

	FDkInventoryFastArrayEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	InventoryComponent->AddRepSubObj(NewEntry.Item);
	MarkItemDirty(NewEntry);
	return Item;
}

void FDkInventoryFastArray::RemoveEntry(UDkInventoryItem* Item)
{
	for (auto EntryIter = Entries.CreateIterator(); EntryIter; ++EntryIter)
	{
		FDkInventoryFastArrayEntry& Entry = *EntryIter;
		if (Entry.Item == Item)
		{
			EntryIter.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UDkInventoryItem* FDkInventoryFastArray::FindFirstItemByTag(const FGameplayTag& ItemTag)
{
	FDkInventoryFastArrayEntry* FoundItem = Entries.FindByPredicate(
		[ItemTag](const FDkInventoryFastArrayEntry& Entry)
		{
			return IsValid(Entry.Item) && Entry.Item->GetItemManifest().GetItemTag().MatchesTagExact(ItemTag);
		}
	);
	return FoundItem ? FoundItem->Item : nullptr;
}
