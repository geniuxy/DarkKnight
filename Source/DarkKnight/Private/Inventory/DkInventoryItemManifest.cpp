#include "Inventory/DkInventoryItemManifest.h"

#include "Components/DkItemComponent.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Subsytems/DkInventorySubsystem.h"
#include "Widgets/Inventory/DkInventoryItemDescriptionMenu.h"

void FInventoryItemManifest::InitializeFragments(const FDkItemInfo* ItemInfo, int32 InItemStack)
{
	// ItemName
	if (ItemInfo->ItemName.IsValid())
	{
		FInventoryItemTextFragment NewItemTextFragment = FInventoryItemTextFragment(
			FText::FromName(ItemInfo->ItemName)
		);
		AddFragment(NewItemTextFragment);
	}

	// ItemRequiredLevel
	if (ItemInfo->ItemRequiredLevel != INVALID_INDEX)
	{
		FInventoryItemLabeledValueFragment NewItemRequiredLevelFragment = FInventoryItemLabeledValueFragment(
			FText::FromString(TEXT("等级要求")),
			ItemInfo->ItemRequiredLevel,
			DkGameplayTags::Dk_Inventory_Fragment_RequiredLevel
		);
		AddFragment(NewItemRequiredLevelFragment);
	}

	// ItemDescription
	if (!ItemInfo->ItemDescription.IsEmpty())
	{
		FInventoryItemTextFragment NewItemDescriptionFragment = FInventoryItemTextFragment(
			ItemInfo->ItemDescription
		);
		AddFragment(NewItemDescriptionFragment);
	}

	// ItemIcon
	if (IsValid(ItemInfo->ItemIcon))
	{
		FInventoryItemImageFragment NewItemImageFragment = FInventoryItemImageFragment(
			ItemInfo->ItemIcon
		);
		AddFragment(NewItemImageFragment);
	}

	// ItemPrice
	if (ItemInfo->ItemPrice != INVALID_INDEX)
	{
		FInventoryItemLabeledValueFragment NewItemSellPriceFragment = FInventoryItemLabeledValueFragment(
			FText::FromString(TEXT("售价")),
			ItemInfo->ItemPrice,
			DkGameplayTags::Dk_Inventory_Fragment_SellValue
		);
		AddFragment(NewItemSellPriceFragment);
	}

	// TODO: ItemWeight

	// Stack
	if (ItemInfo->MaxStack != INVALID_INDEX && InItemStack > 0)
	{
		FInventoryItemStackableFragment NewItemStackableFragment = FInventoryItemStackableFragment(
			InItemStack, ItemInfo->MaxStack
		);
		AddFragment(NewItemStackableFragment);
	}

	// TODO: 生成词条
}

UDkInventoryItem* FInventoryItemManifest::Manifest(UObject* NewOuter) // 这函数在ItemComponent转换为UDkInventoryItem时调用
{
	UDkInventoryItem* Item = NewObject<UDkInventoryItem>(NewOuter, UDkInventoryItem::StaticClass());
	Item->SetItemManifest(*this); // 复制了一份Manifest给新Item的ItemManifest

	// 对新创建Item的ItemManifest的Fragments进行Manifest操作
	for (auto& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();
	}

	// 因为老ItemComponent的ItemManifest已被复制，所以可以被清空了
	ClearFragments();

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

void FInventoryItemManifest::AssimilateInventoryFragments(UDkInventoryCompositeBase* Composite) const
{
	const auto& InventoryItemFragments = GetAllFragmentsOfType<FInventoryItemFragment>();
	for (const auto& Fragment : InventoryItemFragments)
	{
		Composite->ApplyFunction(
			[Fragment](UDkInventoryCompositeBase* Widget)
			{
				Fragment->Assimilate(Widget);
			}
		);

		if (Fragment->GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_Consumable)
		{
			if (static_cast<const FInventoryItemConsumableFragment*>(Fragment)->HasOptionalStats())
			{
				if (UDkInventoryItemDescriptionMenu* ItemDescriptionMenu =
					Cast<UDkInventoryItemDescriptionMenu>(Composite))
				{
					ItemDescriptionMenu->ShowOptionalStatTitle();
				}
			}
		}
	}
}

void FInventoryItemManifest::ClearFragments()
{
	for (auto& Fragment : Fragments)
	{
		Fragment.Reset();
	}
	Fragments.Empty();
}
