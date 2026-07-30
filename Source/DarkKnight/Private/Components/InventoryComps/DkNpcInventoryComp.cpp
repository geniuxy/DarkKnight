// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComps/DkNpcInventoryComp.h"

#include "AbilitySystemGlobals.h"
#include "Characters/NPC/DkCharacterMerchant.h"
#include "DataAssets/PA_MerchantInfoGenerics.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "GAS/DkAbilitySystemComponent.h"

UDkNpcInventoryComp::UDkNpcInventoryComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDkNpcInventoryComp::InitializeInventoryComponent()
{
	OwningMerchant = CastChecked<ADkCharacterMerchant>(GetOwner());
	if (OwningMerchant)
	{
		OwnerASC = Cast<UDkAbilitySystemComponent>(
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningMerchant)
		);
	}
}

void UDkNpcInventoryComp::InitInventorySlotArray()
{
	if (!OwningMerchant) return;
	if (!OwningMerchant->HasAuthority()) return;

	int GlobalIndex = 0;
	for (const FInventoryItemCategoryInfo& CategoryInfo : CategoryInfoList)
	{
		for (int iSlotIndex = 0; iSlotIndex < CategoryInfo.Rows * CategoryInfo.Columns; iSlotIndex++)
		{
			FDkInventorySlotEntry& NewEntry = InventorySlotArray.Slots.AddDefaulted_GetRef();
			NewEntry.Category = CategoryInfo.Category;
			NewEntry.GlobalIndex = GlobalIndex;
			NewEntry.BriefInfo = FInventoryItemBriefInfo(iSlotIndex);
			GlobalIndex++;
		}
	}

	InventorySlotArray.MarkArrayDirty();

	AddMerchantShopItems();
}

void UDkNpcInventoryComp::AddMerchantShopItems()
{
	if (!MerchantInfoGenerics) return;
	for (const FShopItemEntry ShopItemEntry : MerchantInfoGenerics->GetShopItemEntries())
	{
		if (UDkInventoryFunctionLibrary::IsItemStackable(ShopItemEntry.ItemID))
		{
			UDkInventoryItem* ShopItem = UDkInventoryFunctionLibrary::SpawnInventoryItemById(
				this, ShopItemEntry.ItemID, ShopItemEntry.Stack
			);
			if (IsValid(ShopItem))
			{
				TryAddItem(ShopItem);
			}
		}
		else
		{
			for (int i = 0; i < ShopItemEntry.Stack; ++i)
			{
				UDkInventoryItem* ShopItem = UDkInventoryFunctionLibrary::SpawnInventoryItemById(
					this, ShopItemEntry.ItemID, 0
				);
				if (IsValid(ShopItem))
				{
					TryAddItem(ShopItem);
				}
			}
		}
	}
}
