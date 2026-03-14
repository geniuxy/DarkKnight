// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Equipment/DkInventoryEquipmentSlot.h"

#include "CommonLazyImage.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/DkInventoryComponent.h"
#include "Components/OverlaySlot.h"
#include "Inventory/DkInventoryItem.h"
#include "Inventory/DkInventoryItemFragment.h"
#include "Subsytems/DkInventorySubsystem.h"
#include "Widgets/Inventory/DkInventoryItemDescriptionMenu.h"

FReply UDkInventoryEquipmentSlot::NativeOnMouseButtonDown(
	const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GridSlotClicked.Broadcast(TileIndex, InMouseEvent);

	if (ItemDescriptionMenu.IsValid())
	{
		UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get(this);
		checkf(InventorySubsystem, TEXT("InventorySubsystem为空！"));
		UDkInventoryComponent* InventoryComponent = InventorySubsystem->GetCachedInventoryComponent();
		checkf(InventoryComponent, TEXT("InventoryComponent未在InventorySubsystem中注册！"));
		InventoryComponent->OnItemDescriptionMenuRemoved.Broadcast();
		ItemDescriptionMenu = nullptr;
	}

	return FReply::Handled();
}

void UDkInventoryEquipmentSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	CreateItemDescriptionMenu();
}

void UDkInventoryEquipmentSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (ItemDescriptionMenu.IsValid())
	{
		UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get(this);
		checkf(InventorySubsystem, TEXT("InventorySubsystem为空！"));
		UDkInventoryComponent* InventoryComponent = InventorySubsystem->GetCachedInventoryComponent();
		checkf(InventoryComponent, TEXT("InventoryComponent未在InventorySubsystem中注册！"));
		InventoryComponent->OnItemDescriptionMenuRemoved.Broadcast();
		ItemDescriptionMenu = nullptr;
	}
}

void UDkInventoryEquipmentSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// if (ItemDescriptionMenu.IsValid() && ItemDescriptionMenu->IsInViewport())
	// {
	// 	// UDkUIFunctionLibrary::PositionWidgetAtMouse(
	// 	// 	ItemDescriptionMenu.Get(),
	// 	// 	FVector2D{8.f, 8.f},
	// 	// 	false,
	// 	// 	false,
	// 	// 	4
	// 	// );
	//
	// 	FVector2D MousePos;
	// 	if (UGameViewportClient* VP = GetWorld()->GetGameViewport())
	// 	{
	// 		VP->GetMousePosition(MousePos); // 系统硬件像素（受 DPI 缩放）
	// 	}
	//
	// 	const FVector2D ActualSize = ItemDescriptionMenu->GetCachedGeometry().GetAbsoluteSize();
	// 	ItemDescriptionMenu->SetPositionInViewport(FVector2D(MousePos.X - ActualSize.X * 0.6f, MousePos.Y));
	// }
}

void UDkInventoryEquipmentSlot::SetBackGroundIcon(const TSoftObjectPtr<UTexture2D>& InTexture)
{
	Image_ItemIcon->SetBrushFromLazyTexture(InTexture);
}

void UDkInventoryEquipmentSlot::SetEquipmentIcon()
{
	if (!GetInventoryItem()) return;

	const FInventoryItemImageFragment* ImageFragment =
		GetInventoryItem()->GetItemManifest().GetFragmentOfType<FInventoryItemImageFragment>();

	Image_ItemIcon->SetBrushFromLazyTexture(ImageFragment->GetIcon());
}

FVector2D UDkInventoryEquipmentSlot::GetIconSize() const
{
	return GetGridSlotSize() - UWidgetLayoutLibrary::SlotAsOverlaySlot(Image_ItemIcon)->GetPadding().GetTopLeft();
}

void UDkInventoryEquipmentSlot::CreateItemDescriptionMenu()
{
	if (!InventoryItem.IsValid()) return;

	if (!ItemDescriptionMenu.IsValid())
	{
		ItemDescriptionMenu = CreateWidget<UDkInventoryItemDescriptionMenu>(this, ItemDescriptionMenuClass);
	}

	// 根据Fragments，同化(渲染)ItemDescription的内容
	InventoryItem->GetItemManifest().AssimilateInventoryFragments(ItemDescriptionMenu.Get());

	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get(this);
	checkf(InventorySubsystem, TEXT("InventorySubsystem为空！"));
	UDkInventoryComponent* InventoryComponent = InventorySubsystem->GetCachedInventoryComponent();
	checkf(InventoryComponent, TEXT("InventoryComponent未在InventorySubsystem中注册！"));
	InventoryComponent->OnItemDescriptionMenuCreated.Broadcast(ItemDescriptionMenu.Get());
}
