#include "Inventory/DkInventoryFastArray.h"

#include "Components/DkInventoryComponent.h"
#include "Inventory/DkInventoryItem.h"


TArray<UDkInventoryItem*> FDkInventoryFastArray::GetAllItems() const
{
	TArray<UDkInventoryItem*> Results;
	Results.Reserve(SerializerItems.Num());
	for (const auto& SerializerItem : SerializerItems)
	{
		if (!IsValid(SerializerItem.Item)) continue;
		Results.Add(SerializerItem.Item);
	}
	return Results;
}

void FDkInventoryFastArray::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	UDkInventoryComponent* InventoryComponent = Cast<UDkInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : RemovedIndices)
	{
		InventoryComponent->OnItemRemoved.Broadcast(SerializerItems[Index].Item);
	}
}

void FDkInventoryFastArray::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	UDkInventoryComponent* InventoryComponent = Cast<UDkInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : AddedIndices)
	{
		InventoryComponent->OnItemAdded.Broadcast(SerializerItems[Index].Item);
	}
}

UDkInventoryItem* FDkInventoryFastArray::AddSerializerItem(UDkItemComponent* ItemComponent)
{
	return nullptr;
}

UDkInventoryItem* FDkInventoryFastArray::AddSerializerItem(UDkInventoryItem* Item)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FDkInventoryFastArraySerializerItem& NewSerializerItem = SerializerItems.AddDefaulted_GetRef();
	NewSerializerItem.Item = Item;

	MarkItemDirty(NewSerializerItem);
	return Item;
}

void FDkInventoryFastArray::RemoveSerializerItem(UDkInventoryItem* Item)
{
	for (auto SerializerItemIter = SerializerItems.CreateIterator(); SerializerItemIter; ++SerializerItemIter)
	{
		FDkInventoryFastArraySerializerItem& SerializerItem = *SerializerItemIter;
		if (SerializerItem.Item == Item)
		{
			SerializerItemIter.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
