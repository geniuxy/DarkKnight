#include "Inventory/DkInventoryItemManifest.h"

#include "DarkKnightDebugHelper.h"
#include "Components/DkItemComponent.h"
#include "FunctionLibrarys/DkCommonFunctionLibrary.h"
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

	// 根据MainEntry生成主词条
	if (!ItemInfo->MainEntry.IsEmpty())
	{
		InitializeItemEntryFragment(ItemInfo->ItemID, ItemInfo->MainEntry, true);
	}

	// 根据SubEntry生成主词条
	if (!ItemInfo->SubEntry.IsEmpty())
	{
		InitializeItemEntryFragment(ItemInfo->ItemID, ItemInfo->SubEntry, false);
	}
}

void FInventoryItemManifest::InitializeItemEntryFragment(const int32 InItemID, const FText& InEntry, bool bMainEntry)
{
	FString EntryStr = InEntry.ToString();
	TArray<FString> EntryList;
	EntryStr.ParseIntoArray(EntryList, TEXT(","), true);

	if ((bMainEntry && EntryList.Num() > MAX_MAIN_ENTRY_NUM) || (!bMainEntry && EntryList.Num() > MAX_SUB_ENTRY_NUM))
	{
		Debug::Print(FString::Printf(TEXT("ItemID[%d]的词条数目超出规定个数！"), InItemID));
		return;
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

			if (EntryID.IsEmpty() || Value.IsEmpty() || UDkCommonFunctionLibrary::IsStringPureNumber(Value))
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

			if (EntryID.IsEmpty() || MinValue.IsEmpty() || UDkCommonFunctionLibrary::IsStringPureNumber(MinValue)
				|| MaxValue.IsEmpty() || UDkCommonFunctionLibrary::IsStringPureNumber(MaxValue))
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
				Debug::Print(FString::Printf(TEXT("EntryID: %s, Value: %s"), *EntryID, *Value));

				AddItemEntryFragment(
					bMainEntry, Index, FCString::Atoi(*EntryID), FCString::Atoi(*Value)
				);
			}
			else
			{
				Debug::Print(FString::Printf(
						TEXT("EntryID: %s, MinValue: %s, MaxValue: %s"), *EntryID, *MinValue, *MaxValue)
				);

				AddItemEntryFragment(
					bMainEntry, Index, FCString::Atoi(*EntryID), FCString::Atoi(*MinValue), FCString::Atoi(*MaxValue)
				);
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
}

void FInventoryItemManifest::AddItemEntryFragment(
	bool bMainEntry, int32 EntryIndex, int32 InEntryID, int32 InMinValue, int32 InMaxValue)
{
	FInventoryItemLabeledValueFragment NewItemEntryFragment = FInventoryItemLabeledValueFragment(
		bMainEntry ? GetMainEntryTagByIndex(EntryIndex) : GetSubEntryTagByIndex(EntryIndex)
	);
	NewItemEntryFragment.SetMinValue(InMinValue);
	NewItemEntryFragment.SetMaxValue(InMinValue);
	if (InMaxValue != INVALID_INDEX)
	{
		NewItemEntryFragment.SetMaxValue(InMaxValue);
	}
	AddFragment(NewItemEntryFragment);
}

FGameplayTag FInventoryItemManifest::GetMainEntryTagByIndex(int32 InIndex)
{
	checkf(InIndex >= 0 && InIndex < MAX_MAIN_ENTRY_NUM, TEXT("主词条的数目不符合规定！"));
	switch (InIndex)
	{
	case 0:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Main_0;
	case 1:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Main_1;
	default:
		break;
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag FInventoryItemManifest::GetSubEntryTagByIndex(int32 InIndex)
{
	checkf(InIndex >= 0 && InIndex < MAX_SUB_ENTRY_NUM, TEXT("子词条的数目不符合规定！"));
	switch (InIndex)
	{
	case 0:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_0;
	case 1:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_1;
	case 2:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_2;
	case 3:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_3;
	default:
		break;
	}
	return FGameplayTag::EmptyTag;
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
