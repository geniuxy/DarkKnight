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

void UDkInventoryComponent::TryAddItem(UDkItemComponent* ItemComponent)
{
	FDkInventorySlotAvailabilityResult AddItemResult = CachedInventoryMenu->HasRoomForItem(ItemComponent);
	if (AddItemResult.TotalRoomToFill == 0)
	{
		OnNoRoomInInventory.Broadcast();
		return; 
	}

	// TODO: 将Item添加到Inventory中
	
}

void UDkInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
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
