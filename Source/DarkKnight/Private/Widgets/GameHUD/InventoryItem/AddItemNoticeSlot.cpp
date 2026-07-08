// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/InventoryItem/AddItemNoticeSlot.h"

#include "CommonTextBlock.h"

void UAddItemNoticeSlot::ConfigureNoticeText(const FText& InText, float InExistTime)
{
	NoticeTextBlock->SetText(InText);

	FTimerHandle RemoveTimeHandle;
	GetWorld()->GetTimerManager().SetTimer(RemoveTimeHandle, FTimerDelegate::CreateLambda([this]()
	{
		RemoveFromParent();
	}), InExistTime, false);
}
