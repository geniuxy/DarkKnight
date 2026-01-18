#pragma once

#include "DkInventorySlotAvailabilty.generated.h"

class UDkInventoryItem;

USTRUCT()
struct FDkInventorySlotAvailability
{
	GENERATED_BODY()

	FDkInventorySlotAvailability()
	{
	}

	FDkInventorySlotAvailability(int32 ItemIndex, int32 InAmountToFill, bool bHasItem)
	{
		Index = ItemIndex;
		AmountToFill = InAmountToFill;
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
