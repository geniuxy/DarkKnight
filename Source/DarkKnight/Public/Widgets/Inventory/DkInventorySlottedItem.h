// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DarkKnight/DarkKnight.h"
#include "DkInventorySlottedItem.generated.h"

class UDkInventoryItemDescriptionMenu;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSlottedItemClicked, int32, GridIndex, const FPointerEvent&, MouseEvent);

class UCommonTextBlock;
class UDkInventoryItem;
class UCommonLazyImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventorySlottedItem : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	//~Begin UUserWidget Function
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~End UUserWidget Function

	UDkInventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UDkInventoryItem* InItem);

	void SetImageBrush(const FSlateBrush& Brush) const;

	void UpdateStackCount(int32 StackCount);

	FSlottedItemClicked OnSlottedItemClicked;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_StackCount;
	//***** Bound Widgets *****//

	/* 鼠标Hover时，显示Item详细信息 */
	void CreateItemDescriptionMenu();

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UDkInventoryItemDescriptionMenu> ItemDescriptionMenuClass;

	UPROPERTY()
	TObjectPtr<UDkInventoryItemDescriptionMenu> ItemDescriptionMenu;
	/********/
	
	TWeakObjectPtr<UDkInventoryItem> InventoryItem;
	int32 GridIndex;
	FIntPoint GridDimension;
	bool bIsStackable = false;

public:
	LIST_DATA_ACCESSOR(int32, GridIndex)
	LIST_DATA_ACCESSOR(FIntPoint, GridDimension)
	LIST_DATA_ACCESSOR(bool, bIsStackable)

	FORCEINLINE UCommonLazyImage* GetImageIcon() const { return Image_Icon; }
};
