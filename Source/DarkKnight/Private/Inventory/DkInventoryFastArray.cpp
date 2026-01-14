#include "Inventory/DkInventoryFastArray.h"

#include "Components/DkInventoryComponent.h"
#include "Inventory/DkInventoryItem.h"


TArray<UDkInventoryItem*> FDkInventoryFastArray::GetAllItems() const
{
	TArray<UDkInventoryItem*> Results;
	Results.Reserve(Entrys.Num());
	for (const auto& Entry : Entrys)
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
		InventoryComponent->OnItemRemoved.Broadcast(Entrys[Index].Item);
	}
}

void FDkInventoryFastArray::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	UDkInventoryComponent* InventoryComponent = Cast<UDkInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : AddedIndices)
	{
		InventoryComponent->OnItemAdded.Broadcast(Entrys[Index].Item);
	}
}

UDkInventoryItem* FDkInventoryFastArray::AddEntry(UDkItemComponent* ItemComponent)
{
	return nullptr;
}

UDkInventoryItem* FDkInventoryFastArray::AddEntry(UDkInventoryItem* Item)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FDkInventoryFastArrayEntry& NewEntry = Entrys.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	MarkItemDirty(NewEntry);
	return Item;
}

void FDkInventoryFastArray::RemoveEntry(UDkInventoryItem* Item)
{
	for (auto EntryIter = Entrys.CreateIterator(); EntryIter; ++EntryIter)
	{
		FDkInventoryFastArrayEntry& Entry = *EntryIter;
		if (Entry.Item == Item)
		{
			EntryIter.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
