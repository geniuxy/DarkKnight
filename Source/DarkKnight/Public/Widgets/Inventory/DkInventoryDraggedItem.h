// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "DkInventoryDraggedItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDraggedItemEvent, const FPointerEvent&, MouseEvent);

class UDkInventoryItem;
class UCommonTextBlock;
class UCommonLazyImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryDraggedItem : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	//~Begin UUserWidget Function
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//~End UUserWidget Function
	
	void SetImageBrush(const FSlateBrush& Brush) const;
	void UpdateStackCount(const int32 Count);

	FGameplayTag GetItemTag() const;

	bool GetIsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bInIsStackable);

	UDkInventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UDkInventoryItem* InItem);

	FDraggedItemEvent OnDraggedItemClicked;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_StackCount;
	//***** Bound Widgets *****//

	TWeakObjectPtr<UDkInventoryItem> InventoryItem;
	int32 PreviousGridIndex;
	FIntPoint GridDimension;
	bool bIsStackable = false;
	int32 StackCount = 0;

public:
	LIST_DATA_ACCESSOR(int32, PreviousGridIndex)
	LIST_DATA_ACCESSOR(FIntPoint, GridDimension)
	LIST_DATA_ACCESSOR(int32, StackCount)
};
