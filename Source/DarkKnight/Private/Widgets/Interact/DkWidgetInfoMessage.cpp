// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Interact/DkWidgetInfoMessage.h"

#include "CommonTextBlock.h"

void UDkWidgetInfoMessage::SetMessage(const FText& Message)
{
	CommonText_Message->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(
		MessageTimer,
		[this]()
		{
			MessageHide();
			bIsMessageActive = false;
		},
		MessageLifeTime,
		false
	);
}

void UDkWidgetInfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonText_Message->SetText(FText::GetEmpty());
	MessageHide();
}
