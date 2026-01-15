// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkInventoryComponent.h"

#include "DkGameplayTags.h"
#include "Characters/DkCharacterHero.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"
#include "Widgets/Inventory/DkWidgetInventoryMenu.h"

UDkInventoryComponent::UDkInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDkInventoryComponent::ConstructInventoryMenu()
{
	OwningCharacter = CastChecked<ADkCharacterHero>(GetOwner());
	if (!OwningCharacter->IsLocallyControlled())
	{
		return;
	}
	
	UDkUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_GameMenu,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_GameMenu),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::AfterPush)
			{
				UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly);
				CachedInventoryMenu = CastChecked<UDkWidgetGameMenuScreen>(PushedWidget)->GetInventoryMenu();
			}
		}
	);
}

void UDkInventoryComponent::TryAddItem(UDkItemComponent* ItemComponent)
{
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

void UDkInventoryComponent::Server_AddNewItem_Implementation(UDkItemComponent* ItemComponent, int32 StackCount)
{
}

void UDkInventoryComponent::Server_AddStacksToItem_Implementation(
	UDkItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
}

void UDkInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}
