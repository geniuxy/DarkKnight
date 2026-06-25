// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Interact/DkWidgetInteractBox.h"

#include "CommonTextBlock.h"

void UDkWidgetInteractBox::SetMessage(const FString& Message)
{
	CommonText_Message->SetText(FText::FromString(Message));
}

void UDkWidgetInteractBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetVisibility(ESlateVisibility::Collapsed);
}
