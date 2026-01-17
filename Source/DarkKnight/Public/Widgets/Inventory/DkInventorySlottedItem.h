// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DarkKnight/DarkKnight.h"
#include "DkInventorySlottedItem.generated.h"

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
	UDkInventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UDkInventoryItem* InItem);
	void SetImageBrush(const FSlateBrush& Brush) const;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_Icon;
	//***** Bound Widgets *****//
	
	int32 GridIndex;
	FIntPoint GridDimension;
	TWeakObjectPtr<UDkInventoryItem> InventoryItem;
	bool bIsStackable = false;

public:
	LIST_DATA_ACCESSOR(int32, GridIndex)
	LIST_DATA_ACCESSOR(FIntPoint, GridDimension)
	LIST_DATA_ACCESSOR(bool, bIsStackable)

	FORCEINLINE UCommonLazyImage* GetImageIcon() const { return Image_Icon; }
};
