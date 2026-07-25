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

	UPROPERTY()
	int32 Index = INDEX_NONE;
	UPROPERTY()
	int32 AmountToFill = 0;
	UPROPERTY()
	bool bItemAtIndex = false;
};

USTRUCT()
struct FDkInventorySlotAvailabilityResult
{
	GENERATED_BODY()

	FDkInventorySlotAvailabilityResult()
	{
	}

	UPROPERTY()
	UDkInventoryItem* Item = nullptr;
	UPROPERTY()
	int32 TotalRoomToFill = 0;
	UPROPERTY()
	int32 Remainder = 0;
	UPROPERTY()
	bool bStackable = false;
	UPROPERTY()
	TArray<FDkInventorySlotAvailability> SlotAvailabilities;
};
