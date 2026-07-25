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
//
// void UDkPlayerInventoryComp::RequestMoveItem(
// 	EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 StackCount)
// {
// 	Server_MoveItem(Category, FromSlot, ToSlot, StackCount);
// }
//
// void UDkPlayerInventoryComp::RequestMoveItemFromEquipment(
// 	EInventoryItemCategory Category, UDkInventoryItem* Item, int32 ToSlot, int32 StackCount)
// {
// 	Server_MoveItemFromEquipment(Category, Item, ToSlot, StackCount);
// }
//
// void UDkPlayerInventoryComp::RequestSwapItems(
// 	EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA)
// {
// 	Server_SwapItems(Category, SlotA, SlotB, StackCountFromA);
// }
//
// void UDkPlayerInventoryComp::Server_MoveItem_Implementation(
// 	EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 StackCount)
// {
// 	ApplyMoveItem(Category, FromSlot, ToSlot, StackCount);
// }
//
// bool UDkPlayerInventoryComp::Server_MoveItem_Validate(
// 	EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 StackCount)
// {
// 	if (FromSlot < 0 || ToSlot < 0 || FromSlot == ToSlot)
// 	{
// 		return false;
// 	}
// 	TArray<FDkInventorySlotEntry*> SlotEntries = InventorySlotArray.FindByCategory(Category);
// 	if (SlotEntries.IsEmpty()) return false;
// 	if (!SlotEntries.IsValidIndex(FromSlot) || !SlotEntries.IsValidIndex(ToSlot)) return false;
// 	if (!SlotEntries[FromSlot]->BriefInfo.IsValid()) return false;
// 	if (SlotEntries[FromSlot]->BriefInfo.StackCount < StackCount) return false;
// 	if (!SlotEntries[ToSlot]->BriefInfo.IsEmpty() &&
// 		SlotEntries[FromSlot]->BriefInfo.InventoryItem != SlotEntries[ToSlot]->BriefInfo.InventoryItem)
// 	{
// 		return false;
// 	}
//
// 	if (SlotEntries[FromSlot]->BriefInfo.InventoryItem)
// 	{
// 		FGameplayTag ItemTag = SlotEntries[FromSlot]->BriefInfo.InventoryItem->GetItemManifest().GetItemTag();
// 		if (!InventoryList.FindFirstItemByTag(ItemTag)) return false;
// 	}
//
// 	return true;
// }
//
// void UDkPlayerInventoryComp::Server_MoveItemFromEquipment_Implementation(
// 	EInventoryItemCategory Category, UDkInventoryItem* Item, int32 ToSlot, int32 StackCount)
// {
// 	ApplyMoveItemFromEquipment(Category, Item, ToSlot, StackCount);
// }
//
// bool UDkPlayerInventoryComp::Server_MoveItemFromEquipment_Validate(
// 	EInventoryItemCategory Category, UDkInventoryItem* Item, int32 ToSlot, int32 StackCount)
// {
// 	if (ToSlot < 0) return false;
// 	TArray<FDkInventorySlotEntry*> SlotEntries = InventorySlotArray.FindByCategory(Category);
// 	if (SlotEntries.IsEmpty()) return false;
// 	if (!SlotEntries.IsValidIndex(ToSlot)) return false;
// 	if (!SlotEntries[ToSlot]->BriefInfo.IsEmpty()) return false;
//
// 	return true;
// }
//
// void UDkPlayerInventoryComp::Server_SwapItems_Implementation(
// 	EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA)
// {
// 	ApplySwapItems(Category, SlotA, SlotB, StackCountFromA);
// }
//
// bool UDkPlayerInventoryComp::Server_SwapItems_Validate(
// 	EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA)
// {
// 	return true;
// }
//
// void UDkPlayerInventoryComp::ApplyMoveItem(
// 	EInventoryItemCategory Category, int32 FromSlot, int32 ToSlot, int32 MoveStackCount)
// {
// 	// 根据 Category 获取对应槽位数组
// 	TArray<FDkInventorySlotEntry*> SlotEntries = InventorySlotArray.FindByCategory(Category);
// 	if (SlotEntries.IsEmpty()) return;
//
// 	FDkInventorySlotEntry* TargetSlotEntry = SlotEntries[ToSlot];
// 	FDkInventorySlotEntry* SourceSlotEntry = SlotEntries[FromSlot];
// 	FInventoryItemBriefInfo TargetItemInfo = TargetSlotEntry->BriefInfo;
// 	FInventoryItemBriefInfo SourceItemInfo = SourceSlotEntry->BriefInfo;
//
// 	// 执行移动
// 	FInventoryItemBriefInfo NewTargetItemInfo = FInventoryItemBriefInfo(ToSlot);
// 	NewTargetItemInfo.InventoryItem = SourceItemInfo.InventoryItem;
// 	NewTargetItemInfo.StackCount = MoveStackCount + TargetItemInfo.StackCount;
// 	TargetSlotEntry->BriefInfo = NewTargetItemInfo;
//
// 	FInventoryItemBriefInfo NewSourceItemInfo = FInventoryItemBriefInfo(FromSlot);
// 	if (SourceItemInfo.StackCount > MoveStackCount)
// 	{
// 		NewSourceItemInfo.InventoryItem = SourceItemInfo.InventoryItem;
// 		NewSourceItemInfo.StackCount = SourceItemInfo.StackCount - MoveStackCount;
// 	}
// 	SourceSlotEntry->BriefInfo = NewSourceItemInfo;
//
// 	InventorySlotArray.MarkItemDirty(*TargetSlotEntry);
// 	InventorySlotArray.MarkItemDirty(*SourceSlotEntry);
// }
//
// void UDkPlayerInventoryComp::ApplyMoveItemFromEquipment(
// 	EInventoryItemCategory Category, UDkInventoryItem* Item, int32 ToSlot, int32 MoveStackCount)
// {
// 	// 根据 Category 获取对应槽位数组
// 	TArray<FDkInventorySlotEntry*> SlotEntries = InventorySlotArray.FindByCategory(Category);
// 	if (SlotEntries.IsEmpty()) return;
//
// 	FDkInventorySlotEntry* TargetSlotEntry = SlotEntries[ToSlot];
// 	FInventoryItemBriefInfo TargetItemInfo = TargetSlotEntry->BriefInfo;
//
// 	// 执行移动
// 	FInventoryItemBriefInfo NewTargetItemInfo = FInventoryItemBriefInfo(ToSlot);
// 	NewTargetItemInfo.InventoryItem = Item;
// 	NewTargetItemInfo.StackCount = MoveStackCount + TargetItemInfo.StackCount;
// 	TargetSlotEntry->BriefInfo = NewTargetItemInfo;
// 	
// 	InventorySlotArray.MarkItemDirty(*TargetSlotEntry);
// }
//
// void UDkPlayerInventoryComp::ApplySwapItems(EInventoryItemCategory Category, int32 SlotA, int32 SlotB, int32 StackCountFromA)
// {
// 	// 根据 Category 获取对应槽位数组
// 	TArray<FDkInventorySlotEntry*> SlotEntries = InventorySlotArray.FindByCategory(Category);
// 	if (SlotEntries.IsEmpty()) return;
//
// 	FDkInventorySlotEntry* SlotAEntry = SlotEntries[SlotA];
// 	FDkInventorySlotEntry* SlotBEntry = SlotEntries[SlotB];
// 	FInventoryItemBriefInfo SlotAItemInfo = SlotAEntry->BriefInfo;
// 	FInventoryItemBriefInfo SlotBItemInfo = SlotBEntry->BriefInfo;
//
// 	// 执行移动
// 	FInventoryItemBriefInfo NewSlotAItemInfo = FInventoryItemBriefInfo(SlotA);
// 	if (SlotAItemInfo.StackCount > StackCountFromA)
// 	{
// 		NewSlotAItemInfo.InventoryItem = SlotAItemInfo.InventoryItem;
// 		NewSlotAItemInfo.StackCount = SlotAItemInfo.StackCount - StackCountFromA;
// 	}
// 	else
// 	{
// 		NewSlotAItemInfo.InventoryItem = SlotBItemInfo.InventoryItem;
// 		NewSlotAItemInfo.StackCount = SlotBItemInfo.StackCount;
// 	}
// 	SlotAEntry->BriefInfo = NewSlotAItemInfo;
//
// 	FInventoryItemBriefInfo NewSlotBItemInfo = FInventoryItemBriefInfo(SlotB);
// 	NewSlotBItemInfo.InventoryItem = SlotAItemInfo.InventoryItem;
// 	NewSlotBItemInfo.StackCount = StackCountFromA;
// 	SlotBEntry->BriefInfo = NewSlotBItemInfo;
//
// 	InventorySlotArray.MarkItemDirty(*SlotAEntry);
// 	InventorySlotArray.MarkItemDirty(*SlotBEntry);
// }
