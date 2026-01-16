// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
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

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_GridSlot;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_GridSlot;
	//***** Bound Widgets *****//

	int32 TileIndex;
};
