// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/DkWidgetPickUpBox.h"

#include "CommonTextBlock.h"

void UDkWidgetPickUpBox::ShowPickUpMessage(const FString& Message)
{
	CommonText_PickUpMessage->SetText(FText::FromString(Message));

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDkWidgetPickUpBox::HidePickUpMessage()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UDkWidgetPickUpBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bSupportsActivationFocus = false; // 该设置可使Widget不会阻止鼠标等输入
}
