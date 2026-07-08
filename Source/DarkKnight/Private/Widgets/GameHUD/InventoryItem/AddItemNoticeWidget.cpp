// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/InventoryItem/AddItemNoticeWidget.h"

#include "Components/DkInventoryComponent.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Subsytems/DkUISubsystem.h"
#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"
#include "Widgets/GameHUD/InventoryItem/AddItemNoticeSlot.h"

void UAddItemNoticeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OwnerInventoryComp = UDkInventorySubsystem::Get()->GetCachedInventoryComponent();
	OwnerInventoryComp->OnAddItemNotice.AddDynamic(this, &ThisClass::AddItemNotice);
}

void UAddItemNoticeWidget::AddItemNotice(const FText& InItemName, int InItemCount)
{
	FItemNoticeInfo ItemNoticeInfo;
	ItemNoticeInfo.ItemName = InItemName;
	ItemNoticeInfo.ItemCount = InItemCount;

	CachedItemNoticeList.Add(ItemNoticeInfo);

	GetWorld()->GetTimerManager().SetTimer(ItemNoticeTimerHandle, this, &ThisClass::TryPlayItemNotice, 0.2f, true, 0.f);
}

void UAddItemNoticeWidget::TryPlayItemNotice()
{
	if (CachedItemNoticeList.IsEmpty()) return;
	if (UDkUISubsystem::Get(this)->IsGameMenuOpen()) return;

	UAddItemNoticeSlot* AddItemNoticeSlot = CreateWidget<UAddItemNoticeSlot>(this, ItemNoticeSlotClass);
	AddItemNoticeBox->AddChildToVerticalBox(AddItemNoticeSlot);

	FFormatNamedArguments Args;
	Args.Add(TEXT("A"), CachedItemNoticeList[0].ItemName);
	Args.Add(TEXT("B"), CachedItemNoticeList[0].ItemCount);
	FText NoticeText = FText::Format(NoticeTips, Args);
	AddItemNoticeSlot->ConfigureNoticeText(NoticeText, NoticeDuration);
	UGameplayStatics::PlaySound2D(this, ItemGainedSound);

	CachedItemNoticeList.RemoveAt(0);
	if (CachedItemNoticeList.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(ItemNoticeTimerHandle);
	}
}
