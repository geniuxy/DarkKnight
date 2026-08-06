// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/ShopMenu/DkWidgetShopMenuScreen.h"

#include "Components/DkNpcDialogComponent.h"
#include "ICommonInputModule.h"
#include "Characters/NPC/DkCharacterMerchant.h"
#include "Components/InventoryComps/DkNpcInventoryComp.h"
#include "Components/InventoryComps/DkPlayerInventoryComp.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"
#include "Subsytems/GameInstanceSubsystems/DkNpcSubsystem.h"
#include "Widgets/Inventory/DkWidgetInventoryMenu.h"

void UDkWidgetShopMenuScreen::ConfigureShopMenu(int InNpcId, int InDialogId)
{
	if (!UDkNpcSubsystem::Get(this)->GetNpcInfo().Contains(InNpcId)) return;

	NextDialogId = InDialogId;

	CurNpcId = InNpcId;
	UDkInventorySubsystem::Get()->SetCurMerchantNpcId(InNpcId);

	if (ADkCharacterMerchant* Merchant =
		Cast<ADkCharacterMerchant>(UDkNpcSubsystem::Get(this)->GetNpcInfo().FindRef(InNpcId).NpcActor))
	{
		UDkNpcInventoryComp* MerchantInventoryComp = Merchant->GetMerchantInventoryComp();
		WBP_MerchantInventoryMenu->SetInventoryComponent(MerchantInventoryComp);
		WBP_MerchantInventoryMenu->SetIsMerchantInventoryMenu(true);
	}
	UDkPlayerInventoryComp* OwnerInventoryComp = UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer());
	WBP_PlayerInventoryMenu->SetInventoryComponent(OwnerInventoryComp);
	WBP_PlayerInventoryMenu->SetIsShopping(true);
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
		CachedNpcDialogComponent = UDkNpcSubsystem::Get(this)->GetCachedNpcDialogComp();
	}

	return CachedNpcDialogComponent;
}

void UDkWidgetShopMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
