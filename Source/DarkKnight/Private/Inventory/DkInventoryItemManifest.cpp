#include "Inventory/DkInventoryItemManifest.h"

#include "Components/DkItemComponent.h"
#include "Inventory/DkInventoryItem.h"

UDkInventoryItem* FInventoryItemManifest::Manifest(UObject* NewOuter)
{
	UDkInventoryItem* Item = NewObject<UDkInventoryItem>(NewOuter, UDkInventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	return Item;
}

void FInventoryItemManifest::SpawnPickUpActor(
	const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!IsValid(PickUpActorClass) || !IsValid(WorldContextObject)) return;

	AActor* SpawnActor =
		WorldContextObject->GetWorld()->SpawnActor<AActor>(PickUpActorClass, SpawnLocation, SpawnRotation);
	if (!IsValid(SpawnActor)) return;

	UDkItemComponent* ItemComponent = SpawnActor->FindComponentByClass<UDkItemComponent>();
	check(ItemComponent);

	ItemComponent->InitItemManifest(*this);
}
