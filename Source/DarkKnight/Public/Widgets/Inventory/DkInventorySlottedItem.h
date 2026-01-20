// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DarkKnight/DarkKnight.h"
#include "DkInventorySlottedItem.generated.h"

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
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

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
