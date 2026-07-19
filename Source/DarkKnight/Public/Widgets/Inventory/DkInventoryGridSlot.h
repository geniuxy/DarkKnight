// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkEnums.h"
#include "DkInventoryGridSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEvent, int32, GridIndex, const FPointerEvent&, MouseEvent);

class UDkInventoryItemDescriptionMenu;
class UCommonTextBlock;
class UDkInventoryItem;
class USizeBox;
class UCommonLazyImage;
/**
 * 背包的网格Widget(一格)
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryGridSlot : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	//~Begin UUserWidget Function
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	//~End UUserWidget Function

	FVector2D GetGridSlotSize() const;

	void SetGridSlotSize(float InSlotSize);

	EInventoryGridSlotState GetGridSlotState() const { return GridSlotState; }

	virtual void SetUnoccupiedBrush();
	virtual void SetOccupiedBrush();
	virtual void SetEnabledBrush();
	virtual void SetDisabledBrush();
	virtual void SetGrayedOutBrush();
	void SetDefaultItemIcon() const;
	void SetItemIcon(const FSlateBrush& InBrush) const;

	void SetItemStackNum(int32 InStack);

	UDkInventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UDkInventoryItem* InItem);

	FGridSlotEvent GridSlotClicked;
	FGridSlotEvent GridSlotHovered;
	FGridSlotEvent GridSlotUnhovered;

protected:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_GridSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_GridSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_ItemIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Text_StackNum;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush UnoccupiedBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush OccupiedBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush EnabledBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush DisabledBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush GrayedOutBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush DefaultItemIconBrush;

	/* 鼠标Hover时，显示Item详细信息 */
	void CreateItemDescriptionMenu();

	UPROPERTY(EditAnywhere, Category="Inventory")
	bool bShouldCreateItemDescriptionMenu = false;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UDkInventoryItemDescriptionMenu> ItemDescriptionMenuClass;

	TWeakObjectPtr<UDkInventoryItemDescriptionMenu> ItemDescriptionMenu;
	/********/

	EInventoryGridSlotState GridSlotState;

	TWeakObjectPtr<UDkInventoryItem> InventoryItem;

	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 TileIndex = INDEX_NONE;
	
	int32 StackCount = 0;
	int32 UpperLeftIndex = INDEX_NONE;
	bool bAvailable = true;

public:
	LIST_DATA_ACCESSOR(int32, TileIndex)
	LIST_DATA_ACCESSOR(int32, StackCount)
	LIST_DATA_ACCESSOR(int32, UpperLeftIndex)
	FORCEINLINE bool IsAvailable() const { return bAvailable; }
	FORCEINLINE void SetIsAvailable(bool bInIsAvailable) { bAvailable = bInIsAvailable; }
};
