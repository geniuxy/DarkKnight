// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"

#include "ICommonInputModule.h"
#include "Components/DkInventoryComponent.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/Inventory/DkInventoryDraggedItem.h"

void UDkWidgetGameMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);

	InventoryComponent = UDkInventoryFunctionLibrary::GetInventoryComponent(GetOwningPlayer());
	// 绑定DraggedItem创建相关的回调
	InventoryComponent->OnDraggedItemCreated.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemCreated);
	InventoryComponent->OnDraggedItemRemoved.AddUniqueDynamic(this, &ThisClass::HandleDraggedItemRemoved);
}

void UDkWidgetGameMenuScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 拖拽Item，跟随鼠标移动
	if (DraggedItem.IsValid() && DraggedItem->IsInViewport())
	{
		// 全程用硬件像素，以将鼠标定位到DraggedItem中心
		FVector2D MousePos;
		if (UGameViewportClient* VP = GetWorld()->GetGameViewport())
		{
			VP->GetMousePosition(MousePos); // 系统硬件像素（受 DPI 缩放）
		}

		const FVector2D ActualSize = DraggedItem->GetCachedGeometry().GetAbsoluteSize();
		DraggedItem->SetPositionInViewport(MousePos - ActualSize * 0.5f);
	}
}

void UDkWidgetGameMenuScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);
}

void UDkWidgetGameMenuScreen::HandleDraggedItemCreated(UDkInventoryDraggedItem* InDraggedItem)
{
	if (!IsValid(InDraggedItem)) return;
	
	if (DraggedItem.IsValid())
	{
		DraggedItem = nullptr;
	}
	DraggedItem = InDraggedItem;
}

void UDkWidgetGameMenuScreen::HandleDraggedItemRemoved()
{
	DraggedItem = nullptr;
}

void UDkWidgetGameMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
