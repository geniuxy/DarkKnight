// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventorySlottedItem.h"
#include "Widgets/Inventory/DkInventoryItemDescriptionMenu.h"
#include "Inventory/DkInventoryItem.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"

FReply UDkInventorySlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlottedItemClicked.Broadcast(GridIndex, InMouseEvent);

	if (IsValid(ItemDescriptionMenu))
	{
		ItemDescriptionMenu->RemoveFromParent();
		ItemDescriptionMenu = nullptr;
	}

	return FReply::Handled();
}

void UDkInventorySlottedItem::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	CreateItemDescriptionMenu();

	// TODO: 如果有对应类型的装备已装备，则在左边或者右边显示其DescriptionMenu
}

void UDkInventorySlottedItem::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	if (IsValid(ItemDescriptionMenu))
	{
		ItemDescriptionMenu->RemoveFromParent();
		ItemDescriptionMenu = nullptr;
	}
}

void UDkInventorySlottedItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// if (ItemDescriptionMenu && ItemDescriptionMenu->IsInViewport())
	// {
	// 	// 全程用硬件像素，以将DescriptionMenu定位到鼠标附近
	// 	FVector2D MousePos;
	// 	if (UGameViewportClient* VP = GetWorld()->GetGameViewport())
	// 	{
	// 		VP->GetMousePosition(MousePos); // 系统硬件像素（受 DPI 缩放）
	// 	}
	// 	ItemDescriptionMenu->SetPositionInViewport(MousePos + FVector2D(1.f, 1.f));
	// }

	if (ItemDescriptionMenu && ItemDescriptionMenu->IsInViewport())
	{
		UDkUIFunctionLibrary::PositionWidgetAtMouse(
			ItemDescriptionMenu,
			FVector2D{8.f, 8.f},
			true,
			true,
			4
		);
	}
}

UDkInventoryItem* UDkInventorySlottedItem::GetInventoryItem() const
{
	return InventoryItem.IsValid() ? InventoryItem.Get() : nullptr;
}

void UDkInventorySlottedItem::SetInventoryItem(UDkInventoryItem* InItem)
{
	InventoryItem = InItem;
}

void UDkInventorySlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UDkInventorySlottedItem::UpdateStackCount(int32 StackCount)
{
	if (StackCount > 0)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
		Text_StackCount->SetText(FText::AsNumber(StackCount));
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDkInventorySlottedItem::CreateItemDescriptionMenu()
{
	if (!InventoryItem.IsValid()) return;
	
	if (!IsValid(ItemDescriptionMenu))
	{
		ItemDescriptionMenu = CreateWidget<UDkInventoryItemDescriptionMenu>(this, ItemDescriptionMenuClass);
	}

	// 根据Fragments，同化(渲染)ItemDescription的内容
	InventoryItem->GetItemManifest().AssimilateInventoryFragments(ItemDescriptionMenu);
	
	ItemDescriptionMenu->AddToViewport();
}
