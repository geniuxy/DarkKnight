// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Interact/DkWidgetPickUpBox.h"

#include "CommonTextBlock.h"

void UDkWidgetPickUpBox::SetMessage(const FString& Message)
{
	CommonText_PickUpMessage->SetText(FText::FromString(Message));
}

void UDkWidgetPickUpBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);
}
