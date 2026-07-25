// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComps/DkPlayerInventoryComp.h"

#include "DkGameplayTags.h"
#include "Characters/DkCharacterHero.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"
#include "Widgets/Inventory/DkWidgetInventoryMenu.h"

void UDkPlayerInventoryComp::ConstructInventoryMenu()
{
	if (OwningCharacter.IsValid() && !OwningCharacter->IsLocallyControlled())
	{
		return;
	}

	UDkUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_GameMenu,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_GameMenu),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly);
			}
			if (InPushState == EAsyncPushWidgetState::AfterPush)
			{
				UDkWidgetGameMenuScreen* GameMenuScreen = CastChecked<UDkWidgetGameMenuScreen>(PushedWidget);
				GameMenuScreen->SetVisibleCenterArea(DkGameplayTags::Dk_Widget_GameMenu_Inventory);
				UDkWidgetInventoryMenu* InventoryMenu = GameMenuScreen->GetInventoryMenu();
				InventoryMenu->SetInventoryComponent(this);
			}
		}
	);
}

void UDkPlayerInventoryComp::RequestMoveItem(EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot,
                                             int32 StackCount)
{
	Server_MoveItem(Category, FromSlot, ToSlot, StackCount);
}

void UDkPlayerInventoryComp::RequestSwapItems(
	EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA)
{
	Server_SwapItems(Category, SlotA, SlotB, StackCountFromA);
}

void UDkPlayerInventoryComp::Server_MoveItem_Implementation(
	EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 StackCount)
{
	ApplyMoveItem(Category, FromSlot, ToSlot, StackCount);
}

bool UDkPlayerInventoryComp::Server_MoveItem_Validate(
	EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 StackCount)
{
	if (FromSlot < 0 || ToSlot < 0 || FromSlot == ToSlot)
	{
		return false;
	}
	FInventoryCategoryItemsArray CategoryItemsArray = GetInventoryCategoryItemsArray();
	TArray<FInventoryItemBriefInfo>* ItemBriefInfos = CategoryItemsArray.FindItems(Category);
	if (!ItemBriefInfos) return false;
	if (!ItemBriefInfos->IsValidIndex(FromSlot) || !ItemBriefInfos->IsValidIndex(ToSlot)) return false;
	if (!(*ItemBriefInfos)[FromSlot].IsValid()) return false;
	if ((*ItemBriefInfos)[FromSlot].StackCount < StackCount) return false;
	if (!(*ItemBriefInfos)[ToSlot].IsEmpty() &&
		(*ItemBriefInfos)[FromSlot].InventoryItem != (*ItemBriefInfos)[ToSlot].InventoryItem)
	{
		return false;
	}

	FGameplayTag ItemTag = (*ItemBriefInfos)[FromSlot].InventoryItem->GetItemManifest().GetItemTag();
	if (!InventoryList.FindFirstItemByTag(ItemTag)) return false;

	return true;
}

void UDkPlayerInventoryComp::Server_SwapItems_Implementation(
	EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA)
{
	ApplySwapItems(Category, SlotA, SlotB, StackCountFromA);
}

bool UDkPlayerInventoryComp::Server_SwapItems_Validate(
	EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA)
{
	return true;
}

void UDkPlayerInventoryComp::ApplyMoveItem(
	EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 MoveStackCount)
{
	FInventoryCategoryItemsArray CategoryItemsArray = GetInventoryCategoryItemsArray();

	// 根据 Category 获取对应槽位数组
	TArray<FInventoryItemBriefInfo>* ItemBriefInfos = CategoryItemsArray.FindItems(Category);
	if (!ItemBriefInfos) return;

	FInventoryItemBriefInfo TargetItemInfo = (*ItemBriefInfos)[ToSlot];
	FInventoryItemBriefInfo SourceItemInfo = (*ItemBriefInfos)[FromSlot];

	// 执行移动
	FInventoryItemBriefInfo NewTargetItemInfo = FInventoryItemBriefInfo(ToSlot);
	NewTargetItemInfo.InventoryItem = SourceItemInfo.InventoryItem;
	NewTargetItemInfo.StackCount = MoveStackCount + TargetItemInfo.StackCount;
	(*ItemBriefInfos)[ToSlot] = NewTargetItemInfo;

	FInventoryItemBriefInfo NewSourceItemInfo = FInventoryItemBriefInfo(FromSlot);
	if (SourceItemInfo.StackCount > MoveStackCount)
	{
		NewSourceItemInfo.InventoryItem = SourceItemInfo.InventoryItem;
		NewSourceItemInfo.StackCount = SourceItemInfo.StackCount - MoveStackCount;
	}
	(*ItemBriefInfos)[FromSlot] = NewSourceItemInfo;

	// 提交变更（触发 Replication）
	CommitCategoryItemsArray(CategoryItemsArray);
}

void UDkPlayerInventoryComp::ApplySwapItems(EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA)
{
	FInventoryCategoryItemsArray CategoryItemsArray = GetInventoryCategoryItemsArray();

	// 根据 Category 获取对应槽位数组
	TArray<FInventoryItemBriefInfo>* ItemBriefInfos = CategoryItemsArray.FindItems(Category);
	if (!ItemBriefInfos) return;

	FInventoryItemBriefInfo SlotAItemInfo = (*ItemBriefInfos)[SlotA];
	FInventoryItemBriefInfo SlotBItemInfo = (*ItemBriefInfos)[SlotB];

	// 执行移动
	FInventoryItemBriefInfo NewSlotAItemInfo = FInventoryItemBriefInfo(SlotA);
	if (SlotAItemInfo.StackCount > StackCountFromA)
	{
		NewSlotAItemInfo.InventoryItem = SlotAItemInfo.InventoryItem;
		NewSlotAItemInfo.StackCount = SlotAItemInfo.StackCount - StackCountFromA;
	}
	else
	{
		NewSlotAItemInfo.InventoryItem = SlotBItemInfo.InventoryItem;
		NewSlotAItemInfo.StackCount = SlotBItemInfo.StackCount;
	}
	(*ItemBriefInfos)[SlotA] = NewSlotAItemInfo;

	FInventoryItemBriefInfo NewSlotBItemInfo = FInventoryItemBriefInfo(SlotB);
	NewSlotBItemInfo.InventoryItem = SlotAItemInfo.InventoryItem;
	NewSlotBItemInfo.StackCount = StackCountFromA;
	(*ItemBriefInfos)[SlotB] = NewSlotBItemInfo;

	// 提交变更（触发 Replication）
	CommitCategoryItemsArray(CategoryItemsArray);
}

void UDkPlayerInventoryComp::CommitCategoryItemsArray(const FInventoryCategoryItemsArray& InCategoryItemsArray)
{
	InventoryCategoryItemsArray = FInstancedStruct::Make(InCategoryItemsArray);
}
