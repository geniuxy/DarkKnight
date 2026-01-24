// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkEnums.h"
#include "DkInventoryGridSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEvent, int32, GridIndex, const FPointerEvent&, MouseEvent);

class UDkInventoryItem;
class USizeBox;
class UCommonLazyImage;
/**
 * 背包的网格Widget
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryGridSlot : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	//~Begin UUserWidget Function
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//~End UUserWidget Function

	void SetGridSlotSize(float InSlotSize);

	EInventoryGridSlotState GetGridSlotState() const { return GridSlotState; }

	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

	UDkInventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UDkInventoryItem* InItem);

	FGridSlotEvent GridSlotClicked;
	FGridSlotEvent GridSlotHovered;
	FGridSlotEvent GridSlotUnhovered;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_GridSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_GridSlot;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSoftObjectPtr<UTexture2D> GridSlotBgImageUnoccupied;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSoftObjectPtr<UTexture2D> GridSlotBgImageOccupied;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSoftObjectPtr<UTexture2D> GridSlotBgImageSelected;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSoftObjectPtr<UTexture2D> GridSlotBgImageGrayedOut;

	EInventoryGridSlotState GridSlotState;

	TWeakObjectPtr<UDkInventoryItem> InventoryItem;
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
