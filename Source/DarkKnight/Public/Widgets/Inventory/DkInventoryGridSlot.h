// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkEnums.h"
#include "DkInventoryGridSlot.generated.h"

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
	void SetGridSlotSize(float InSlotSize);

	EInventoryGridSlotState GetGridSlotState() const { return GridSlotState; }

	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

	UDkInventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UDkInventoryItem* InItem);

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
	int32 TileIndex;
	int32 StackCount;
	int32 UpperLeftIndex = INDEX_NONE;
	bool bAvailable;

public:
	LIST_DATA_ACCESSOR(int32, TileIndex)
	LIST_DATA_ACCESSOR(int32, StackCount)
	LIST_DATA_ACCESSOR(int32, UpperLeftIndex)
	LIST_DATA_ACCESSOR(bool, bAvailable)
};
