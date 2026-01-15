// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkInventoryComponent.h"

#include "DkGameplayTags.h"
#include "Characters/DkCharacterHero.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"
#include "Widgets/Inventory/DkWidgetInventoryMenu.h"

UDkInventoryComponent::UDkInventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UDkInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UDkInventoryComponent::ConstructInventoryMenu()
{
	if (!bHasInitInventoryMenu)
	{
		OwningCharacter = CastChecked<ADkCharacterHero>(GetOwner());
	}
	
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
				if (!bHasInitInventoryMenu)
				{
					CachedInventoryMenu = CastChecked<UDkWidgetGameMenuScreen>(PushedWidget)->GetInventoryMenu();
					PushedWidget->DeactivateWidget();
					bHasInitInventoryMenu = true;
				}
			}
		}
	);
}

void UDkInventoryComponent::TryAddItem(UDkItemComponent* ItemComponent)
{
	if (!CachedInventoryMenu.IsValid()) return;
	FDkInventorySlotAvailabilityResult AddItemResult = CachedInventoryMenu->HasRoomForItem(ItemComponent);
	if (AddItemResult.TotalRoomToFill == 0)
	{
		OnNoRoomInInventory.Broadcast();
		return;
	}

	// 将Item添加到Inventory中
	if (AddItemResult.Item.IsValid() && AddItemResult.bStackable)
	{
		// 为背包中已存在的物品添加堆叠数量。我们只想更新堆叠数量，
		// 而不是创建这种类型的新物品。
		Server_AddStacksToItem(ItemComponent, AddItemResult.TotalRoomToFill, AddItemResult.Remainder);
	}
	else
	{
		// 此物品类型在物品栏中不存在。请创建一个新物品并更新所有相关栏位。
		Server_AddNewItem(ItemComponent, AddItemResult.bStackable ? AddItemResult.TotalRoomToFill : 0);
	}
}

void UDkInventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void UDkInventoryComponent::Server_AddNewItem_Implementation(UDkItemComponent* ItemComponent, int32 StackCount)
{
	UDkInventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);

	if (OwningCharacter.IsValid())
	{
		if (OwningCharacter->GetController()->GetNetMode() == NM_ListenServer ||
			OwningCharacter->GetController()->GetNetMode() == NM_Standalone)
		{
			OnItemAdded.Broadcast(NewItem);
		}
	}
	
	// TODO: 通知 Item Component 销毁 Owner 道具Actor
}

void UDkInventoryComponent::Server_AddStacksToItem_Implementation(
	UDkItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
}

void UDkInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventoryMenu();
}
