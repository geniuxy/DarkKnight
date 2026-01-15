#pragma once
#include "DkTypes/DkEnums.h"

#include "DkInventoryStruct.generated.h"

class UDkInventoryItem;

USTRUCT()
struct FDkInventorySlotAvailability
{
	GENERATED_BODY()

	FDkInventorySlotAvailability()
	{
	}

	FDkInventorySlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem)
	{
		Index = ItemIndex;
		AmountToFill = Room;
		bItemAtIndex = bHasItem;
	}

	int32 Index = INDEX_NONE;
	int32 AmountToFill = 0;
	bool bItemAtIndex = false;
};

USTRUCT()
struct FDkInventorySlotAvailabilityResult
{
	GENERATED_BODY()

	FDkInventorySlotAvailabilityResult()
	{
	}

	TWeakObjectPtr<UDkInventoryItem> Item;
	int32 TotalRoomToFill = 0;
	int32 Remainder = 0;
	bool bStackable = false;
	TArray<FDkInventorySlotAvailability> SlotAvailabilities;
};

// DARKKNIGHT_API 不加的话，默认只在当前模块可见，别的模块(不在同一个 .build.cs 文件里的代码)就链接不到。
USTRUCT(BlueprintType)
struct DARKKNIGHT_API FInventoryItemManifest
{
	GENERATED_BODY()

	UDkInventoryItem* Manifest(UObject* NewOuter);
	EInventoryItemCategory GetItemCategory() const { return ItemCategory; }

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	EInventoryItemCategory ItemCategory = EInventoryItemCategory::None;
};
