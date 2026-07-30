// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/ShopMenu/DkWidgetShopMenuScreen.h"

#include "Components/DkNpcDialogComponent.h"
#include "DarkKnightDebugHelper.h"
#include "ICommonInputModule.h"
#include "Characters/NPC/DkCharacterMerchant.h"
#include "Components/InventoryComps/DkNpcInventoryComp.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Widgets/Inventory/DkWidgetInventoryMenu.h"

void UDkWidgetShopMenuScreen::ConfigureShopMenu(int InNpcId, int InDialogId)
{
	if (!UDkDataSubsystem::Get()->GetNpcInfo().Contains(InNpcId)) return;
	CurNpcId = InNpcId;
	NextDialogId = InDialogId;

	if (ADkCharacterMerchant* Merchant =
		Cast<ADkCharacterMerchant>(UDkDataSubsystem::Get()->GetNpcInfo().FindRef(InNpcId).NpcActor))
	{
		UDkNpcInventoryComp* MerchantInventoryComp = Merchant->GetMerchantInventoryComp();
		WBP_MerchantInventoryMenu->SetInventoryComponent(MerchantInventoryComp);
		WBP_MerchantInventoryMenu->SetIsMerchantInventoryMenu(true);
	}
}

void UDkWidgetShopMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);
}

void UDkWidgetShopMenuScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UDkWidgetShopMenuScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (!GetNpcDialogComponent()) return;

	CachedNpcDialogComponent->OnTriggerNextDialog.Broadcast(NextDialogId);
}

UDkNpcDialogComponent* UDkWidgetShopMenuScreen::GetNpcDialogComponent()
{
	if (!IsValid(CachedNpcDialogComponent))
	{
		AActor* CurNpcActor = UDkDataSubsystem::Get()->GetNpcInfo().FindRef(CurNpcId).NpcActor;
		if (!CurNpcActor)
		{
			Debug::Print(FString::Printf(TEXT("获取商店对应NpcDialogComp时，没找到对应的Npc Actor, Id为: %d"), CurNpcId));
			return nullptr;
		}

		CachedNpcDialogComponent = CurNpcActor->FindComponentByClass<UDkNpcDialogComponent>();
	}

	return CachedNpcDialogComponent;
}

void UDkWidgetShopMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
