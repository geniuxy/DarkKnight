#include "Inventory/DkInventoryItemManifest.h"

#include "DarkKnightDebugHelper.h"
#include "Components/DkItemComponent.h"
#include "FunctionLibrarys/DkCommonFunctionLibrary.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "PickUp/DkPickUpActorBase.h"
#include "Subsytems/DkInventorySubsystem.h"
#include "Widgets/Inventory/DkInventoryItemDescriptionMenu.h"

void FInventoryItemManifest::InitializeFragments(
	const UObject* WorldContextObject, const FDkItemInfo* ItemInfo, int32 InItemStack)
{
	// ItemName
	if (!ItemInfo->ItemName.IsEmpty())
	{
		FInventoryItemTextFragment NewItemTextFragment = FInventoryItemTextFragment(
			ItemInfo->ItemName
		);
		AddFragment(NewItemTextFragment);
	}

	// ItemCategory
	if (GetItemCategory() != EInventoryItemCategory::None)
	{
		FInventoryItemEnumTextFragment NewItemEnumTextFragment = FInventoryItemEnumTextFragment(
			static_cast<int>(GetItemCategory()),
			StaticEnum<EInventoryItemCategory>()->GetPathName()
		);
		AddFragment(NewItemEnumTextFragment);
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
			ItemInfo->ItemDescription, DkGameplayTags::Dk_Inventory_Fragment_ItemDescription
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

	// PickUpActorBPClass
	if (IsValid(ItemInfo->PickUpActorBPClass))
	{
		PickUpActorClass = ItemInfo->PickUpActorBPClass;
	}

	TArray<FItemEntryInfo> MainEntries;
	TArray<FItemEntryInfo> SubEntries;
	// 根据MainEntry生成主词条
	if (!ItemInfo->MainEntry.IsEmpty())
	{
		MainEntries = GetItemEntryInfoList(ItemInfo->ItemID, ItemInfo->MainEntry, true);
	}

	// 根据SubEntry生成主词条
	if (!ItemInfo->SubEntry.IsEmpty())
	{
		SubEntries = GetItemEntryInfoList(ItemInfo->ItemID, ItemInfo->SubEntry, false);
	}

	// EquippedActorBPClass
	if (IsValid(ItemInfo->EquippedActorBPClass) && GetItemCategory() == EInventoryItemCategory::Equipment)
	{
		FInventoryItemEquipmentFragment NewItemEquipmentFragment = FInventoryItemEquipmentFragment(
			ItemInfo->EquippedActorBPClass,
			GetItemTag()
		);
		if (!MainEntries.IsEmpty())
		{
			NewItemEquipmentFragment.UpdateEquipEntries(WorldContextObject, MainEntries, true);
		}
		if (!SubEntries.IsEmpty())
		{
			NewItemEquipmentFragment.UpdateEquipEntries(WorldContextObject, SubEntries, false);
		}
		AddFragment(NewItemEquipmentFragment);
	}

	// TODO: 消耗品相关
}

TArray<FItemEntryInfo> FInventoryItemManifest::GetItemEntryInfoList(
	int32 InItemID, const FText& InEntry, bool bMainEntry)
{
	FString EntryStr = InEntry.ToString();
	TArray<FString> EntryList;
	EntryStr.ParseIntoArray(EntryList, TEXT(","), true);
	TArray<FItemEntryInfo> EntryInfos;

	if ((bMainEntry && EntryList.Num() > MAX_MAIN_ENTRY_NUM) || (!bMainEntry && EntryList.Num() > MAX_SUB_ENTRY_NUM))
	{
		Debug::Print(FString::Printf(TEXT("ItemID[%d]的词条数目超出规定个数！"), InItemID));
		return EntryInfos;
	}

	for (int Index = 0; Index < EntryList.Num(); ++Index)
	{
		const FString& SingleEntry = EntryList[Index];
		if (SingleEntry.IsEmpty())
		{
			Debug::Print(TEXT("检测到空词条，格式错误！"));
			continue;
		}

		TArray<FString> EntryParts;
		SingleEntry.ParseIntoArray(EntryParts, TEXT("/"), true);

		bool bFormatValid = false;
		FString EntryID;
		FString Value;
		FString MinValue;
		FString MaxValue;

		if (EntryParts.Num() == 2)
		{
			bFormatValid = true;
			EntryID = EntryParts[0].TrimStartAndEnd();
			Value = EntryParts[1].TrimStartAndEnd();

			if (EntryID.IsEmpty() || Value.IsEmpty() || !UDkCommonFunctionLibrary::IsStringPureNumber(Value))
			{
				bFormatValid = false;
			}
		}
		else if (EntryParts.Num() == 3)
		{
			bFormatValid = true;
			EntryID = EntryParts[0].TrimStartAndEnd();
			MinValue = EntryParts[1].TrimStartAndEnd();
			MaxValue = EntryParts[2].TrimStartAndEnd();

			if (EntryID.IsEmpty() || MinValue.IsEmpty() || !UDkCommonFunctionLibrary::IsStringPureNumber(MinValue)
				|| MaxValue.IsEmpty() || !UDkCommonFunctionLibrary::IsStringPureNumber(MaxValue))
			{
				bFormatValid = false;
			}
		}
		else
		{
			bFormatValid = false;
		}

		if (bFormatValid)
		{
			Debug::Print(FString::Printf(TEXT("词条[%s]格式正确！"), *SingleEntry));

			if (EntryParts.Num() == 2)
			{
				EntryInfos.Add(FItemEntryInfo(FName(*EntryID), FCString::Atof(*Value), FCString::Atof(*Value)));
			}
			else if (EntryParts.Num() == 3)
			{
				EntryInfos.Add(FItemEntryInfo(FName(*EntryID), FCString::Atof(*MinValue), FCString::Atof(*MaxValue)));
			}
		}
		else
		{
			Debug::Print(FString::Printf(
					TEXT("ItemID[%d]的词条[%s]格式错误！请遵循 EntryID/Value 或 EntryID/MinValue/MaxValue 格式"),
					InItemID, *SingleEntry
				)
			);
		}
	}
	return EntryInfos;
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
