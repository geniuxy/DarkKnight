// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Interact/DkWidgetInteractScreen.h"

#include "Characters/DkCharacterHero.h"
#include "Components/DkInventoryComponent.h"
#include "Widgets/Interact/DkWidgetInfoMessage.h"
#include "Widgets/Interact/DkWidgetPickUpBox.h"

void UDkWidgetInteractScreen::ShowPickUpMessage(const FString& Message)
{
	WBP_PickUpBox->SetMessage(Message);

	WBP_PickUpBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDkWidgetInteractScreen::HidePickUpMessage()
{
	WBP_PickUpBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UDkWidgetInteractScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bSupportsActivationFocus = false;

	UDkInventoryComponent* InventoryComponent =
		CastChecked<ADkCharacterHero>(GetOwningPlayerPawn())->GetInventoryComponent();
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->OnNoRoomInInventory.AddDynamic(this, &ThisClass::ShowNoRoomInInventory);
	}
}

void UDkWidgetInteractScreen::ShowNoRoomInInventory()
{
	if (!IsValid(WBP_InfoMessage)) return;
	WBP_InfoMessage->SetMessage(TEXT("背包中没有足够的空间了"));
}
