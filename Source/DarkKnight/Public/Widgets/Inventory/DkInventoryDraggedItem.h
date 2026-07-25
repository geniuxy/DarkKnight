// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkEnums.h"
#include "DkInventoryDraggedItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDraggedItemEvent, const FPointerEvent&, MouseEvent);

class UDkInventoryItem;
class UCommonTextBlock;
class UCommonLazyImage;
/**
 * 背包拖拽物品后，出现的图标
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryDraggedItem : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	//~Begin UUserWidget Function
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~End UUserWidget Function

	FDraggedItemEvent OnDraggedItemClicked;

	void SetImageBrush(const FSlateBrush& Brush) const;
	void UpdateStackCount(const int32 Count);

	FGameplayTag GetItemTag() const;
	EInventoryItemCategory GetItemCategory() const;

	bool IsPreviousEquipped() const { return bPreviousEquipped; }
	void SetIsPreviousEquipped(bool bInPreviousEquipped) { bPreviousEquipped = bInPreviousEquipped; }

	bool GetIsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bInIsStackable);

	UDkInventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UDkInventoryItem* InItem);

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_StackCount;
	//***** Bound Widgets *****//

	TWeakObjectPtr<UDkInventoryItem> InventoryItem;
	int32 PreviousGridIndex;
	bool bPreviousEquipped;
	FIntPoint GridDimension;
	bool bIsStackable = false;
	int32 StackCount = 0;

public:
	LIST_DATA_ACCESSOR(int32, PreviousGridIndex)
	LIST_DATA_ACCESSOR(FIntPoint, GridDimension)
	LIST_DATA_ACCESSOR(int32, StackCount)
};
