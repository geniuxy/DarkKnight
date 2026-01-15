#include "Inventory/DkInventoryStruct.h"

#include "Inventory/DkInventoryItem.h"

UDkInventoryItem* FInventoryItemManifest::Manifest(UObject* NewOuter)
{
	UDkInventoryItem* Item = NewObject<UDkInventoryItem>(NewOuter, UDkInventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	return Item;
}
