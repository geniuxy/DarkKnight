// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComps/DkPlayerInventoryComp.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DkGameplayTags.h"
#include "Characters/DkCharacterHero.h"
#include "Characters/NPC/DkCharacterMerchant.h"
#include "Components/InventoryComps/DkNpcInventoryComp.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkAttributeSet.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Subsytems/DkUISubsystem.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Subsytems/GameInstanceSubsystems/DkNpcSubsystem.h"
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

void UDkPlayerInventoryComp::Server_TryToSellItem_Implementation(
	int MerchantNpcId, EInventoryItemCategory ItemCategory, int GridIndex, int Count)
{
	if (!UDkNpcSubsystem::Get(this)->GetNpcInfo().Contains(MerchantNpcId)) return;

	FNpcInfo NpcInfo = UDkNpcSubsystem::Get(this)->GetNpcInfo().FindRef(MerchantNpcId);
	ADkCharacterMerchant* Merchant = Cast<ADkCharacterMerchant>(NpcInfo.NpcActor);
	if (!Merchant) return;
	UDkNpcInventoryComp* MerchantInventoryComp = Merchant->GetMerchantInventoryComp();
	if (!MerchantInventoryComp) return;
	UAbilitySystemComponent* MerchantASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Merchant);
	if (!MerchantASC) return;
	if (!OwnerASC.IsValid()) return;

	FDkInventorySlotEntry* SlotEntry = GetInventorySlotArray().FindBySlotIndex(ItemCategory, GridIndex);
	int ItemId = SlotEntry->BriefInfo.InventoryItem->GetItemId();
	UDkInventoryItem* ItemToAdd =
		UDkInventoryFunctionLibrary::SpawnInventoryItemById(this, ItemId, Count);
	const FInventoryItemFragment_LabeledValue* PriceFragment = GetFragment<FInventoryItemFragment_LabeledValue>(
		ItemToAdd, DkGameplayTags::Dk_Inventory_Fragment_SellValue
	);
	float Price = PriceFragment->GetValue() * Count;
	float MerchantCurMoney = MerchantASC->GetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute());
	if (MerchantCurMoney < Price) return;
	
	Server_RemoveItem(ItemCategory, GridIndex, Count);
	MerchantInventoryComp->TryAddItem(ItemToAdd, false);

	// 商人扣钱
	float NewMerchantGold = MerchantASC->GetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute()) - Price;
	MerchantASC->SetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute(), NewMerchantGold);
	
	// 玩家加钱
	float NewPlayerGold = OwnerASC->GetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute()) + Price;
	OwnerASC->SetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute(), NewPlayerGold);

	Client_NotifyToUpdateGrid(MerchantNpcId);
}

bool UDkPlayerInventoryComp::Server_TryToSellItem_Validate(
	int MerchantNpcId, EInventoryItemCategory ItemCategory, int GridIndex, int Count)
{
	return true;
}

void UDkPlayerInventoryComp::Server_TryToBuyItem_Implementation(
	int MerchantNpcId, EInventoryItemCategory ItemCategory, int GridIndex, int Count)
{
	if (!UDkNpcSubsystem::Get(this)->GetNpcInfo().Contains(MerchantNpcId)) return;

	FNpcInfo NpcInfo = UDkNpcSubsystem::Get(this)->GetNpcInfo().FindRef(MerchantNpcId);
	ADkCharacterMerchant* Merchant = Cast<ADkCharacterMerchant>(NpcInfo.NpcActor);
	if (!Merchant) return;
	UDkNpcInventoryComp* MerchantInventoryComp = Merchant->GetMerchantInventoryComp();
	if (!MerchantInventoryComp) return;
	UAbilitySystemComponent* MerchantASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Merchant);
	if (!MerchantASC) return;
	if (!OwnerASC.IsValid()) return;

	FDkInventorySlotEntry* SlotEntry =
		MerchantInventoryComp->GetInventorySlotArray().FindBySlotIndex(ItemCategory, GridIndex);
	int ItemId = SlotEntry->BriefInfo.InventoryItem->GetItemId();
	UDkInventoryItem* ItemToAdd =
		UDkInventoryFunctionLibrary::SpawnInventoryItemById(this, ItemId, Count);
	const FInventoryItemFragment_LabeledValue* PriceFragment = GetFragment<FInventoryItemFragment_LabeledValue>(
		ItemToAdd, DkGameplayTags::Dk_Inventory_Fragment_SellValue
	);
	float Price = PriceFragment->GetValue() * Count;
	float PlayerCurMoney = OwnerASC->GetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute());
	if (PlayerCurMoney < Price) return;
	
	MerchantInventoryComp->Server_RemoveItem(ItemCategory, GridIndex, Count);
	TryAddItem(ItemToAdd, false);

	// 商人加钱
	float NewMerchantGold = MerchantASC->GetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute()) + Price;
	MerchantASC->SetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute(), NewMerchantGold);
	
	// 玩家扣钱
	float NewPlayerGold = OwnerASC->GetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute()) - Price;
	OwnerASC->SetNumericAttributeBase(UDkAttributeSet::GetGoldAttribute(), NewPlayerGold);

	Client_NotifyToUpdateGrid(MerchantNpcId);

	// TODO:不知道为什么这里用GE和ApplyModToAttribute都不能修改商人的金币
	// auto ModifyGold = [this](UAbilitySystemComponent* ASC, float Delta)
	// {
	// 	if (!GE_ModifyGold) return;
	//
	// 	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	// 	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GE_ModifyGold, 1.0f, Context);
	//
	// 	if (Spec.IsValid())
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("=== ASC: %s ==="), *ASC->GetOwner()->GetName());
	//
	// 		// 2. 代码中的属性定义
	// 		FGameplayAttribute CodeAttr = UDkAttributeSet::GetGoldAttribute();
	// 		UE_LOG(LogTemp, Error, TEXT("Code Attr Name: %s"), *CodeAttr.GetName());
	// 		UE_LOG(LogTemp, Error, TEXT("Code Attr Owner: %s"),
	// 		       CodeAttr.GetAttributeSetClass() ? *CodeAttr.GetAttributeSetClass()->GetName() : TEXT("NULL"));
	//
	// 		// 3. ASC 内部状态
	// 		UE_LOG(LogTemp, Error, TEXT("AbilityActorInfo Valid: %s"),
	// 		       ASC->AbilityActorInfo.IsValid() ? TEXT("True") : TEXT("False"));
	// 		if (ASC->AbilityActorInfo.IsValid())
	// 		{
	// 			UE_LOG(LogTemp, Error, TEXT("OwnerActor Valid: %s"),
	// 			       ASC->AbilityActorInfo->OwnerActor.IsValid() ? TEXT("True") : TEXT("False"));
	// 		}
	//
	// 		// 执行
	// 		Spec.Data->SetSetByCallerMagnitude(DkGameplayTags::Dk_SetByCaller_ModifyGold, Delta);
	// 		FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	// 		UE_LOG(LogTemp, Error, TEXT("Handle Valid: %s"), Handle.IsValid() ? TEXT("True") : TEXT("False"));
	// 	}
	// };
	//
	// // 商人加钱
	// ModifyGold(MerchantASC, Price);
	//
	// // 玩家扣钱
	// ModifyGold(OwnerASC.Get(), -Price);
}

bool UDkPlayerInventoryComp::Server_TryToBuyItem_Validate(
	int MerchantNpcId, EInventoryItemCategory ItemCategory, int GridIndex, int Count)
{
	return true;
}

void UDkPlayerInventoryComp::Client_NotifyToUpdateGrid_Implementation(int MerchantNpcId)
{
	// TODO: 不知道为什么有的时候FastArray的变化通知函数不会触发，唉。。。
	FNpcInfo NpcInfo = UDkNpcSubsystem::Get(this)->GetNpcInfo().FindRef(MerchantNpcId);
	ADkCharacterMerchant* Merchant = Cast<ADkCharacterMerchant>(NpcInfo.NpcActor);
	if (!Merchant) return;
	UDkNpcInventoryComp* MerchantInventoryComp = Merchant->GetMerchantInventoryComp();
	if (!MerchantInventoryComp) return;
	MerchantInventoryComp->OnInventorySlotArrayUpdated.Broadcast();

	OnInventorySlotArrayUpdated.Broadcast();
}
