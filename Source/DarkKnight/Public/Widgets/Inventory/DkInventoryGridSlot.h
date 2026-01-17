// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkTypes/DkEnums.h"
#include "DkInventoryGridSlot.generated.h"

class USizeBox;
class UCommonLazyImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryGridSlot : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetTileIndex(int32 Index) { TileIndex = Index; }
	int32 GetTileIndex() const { return TileIndex; }

	void SetGridSlotSize(float InSlotSize);

	EInventoryGridSlotState GetGridSlotState() const { return GridSlotState; }

	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

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

	int32 TileIndex;
};
