// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Interact/DkWidgetInteractScreen.h"

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
}
