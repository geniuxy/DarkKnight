// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkInventoryComponent.h"

#include "DkGameplayTags.h"
#include "Characters/DkCharacterHero.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/DkWidgetActivatableBase.h"

UDkInventoryComponent::UDkInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
			}
		}
	);
}
