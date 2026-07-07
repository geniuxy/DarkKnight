// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"

#include "DkGameplayTags.h"
#include "Characters/DkCharacterHero.h"
#include "Components/DkInventoryComponent.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Subsytems/DkInventorySubsystem.h"

int32 UDkInventoryFunctionLibrary::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint UDkInventoryFunctionLibrary::GetPositionFormIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

UDkInventoryComponent* UDkInventoryFunctionLibrary::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;
	UDkInventoryComponent* InventoryComponent =
		CastChecked<ADkCharacterHero>(PlayerController->GetPawn())->FindComponentByClass<UDkInventoryComponent>();
	return InventoryComponent;
}

FGameplayTag UDkInventoryFunctionLibrary::GetMainEntryTagByIndex(int32 InIndex)
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

FGameplayTag UDkInventoryFunctionLibrary::GetSubEntryTagByIndex(int32 InIndex)
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

UDkInventoryItem* UDkInventoryFunctionLibrary::SpawnInventoryItemById(UObject* NewOuter, int32 InItemId, int InStack)
{
	UDkInventoryItem* Item = NewObject<UDkInventoryItem>(NewOuter, UDkInventoryItem::StaticClass());
	TMap<int, FDkItemInfo> ItemInfoTable = UDkInventorySubsystem::Get(NewOuter)->GetCachedItemTable();
	if (!ItemInfoTable.Contains(InItemId)) return nullptr;

	FDkItemInfo ItemInfo = ItemInfoTable.FindRef(InItemId);
	Item->SetItemManifest(ItemInfo, InStack);

	// 对新创建Item的ItemManifest的Fragments进行Manifest操作
	for (auto& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();
	}
	return Item;
}

bool UDkInventoryFunctionLibrary::IsItemStackable(UObject* WorldContextObject, int32 InItemId)
{
	TMap<int, FDkItemInfo> ItemInfoTable = UDkInventorySubsystem::Get(WorldContextObject)->GetCachedItemTable();
	if (!ItemInfoTable.Contains(InItemId)) return false;

	return ItemInfoTable[InItemId].MaxStack != INDEX_NONE;
}
