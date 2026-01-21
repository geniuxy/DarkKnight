#pragma once

/* Common Types */
UENUM(BlueprintType)
enum class EDkTextJustify : uint8
{
	Left UMETA(DisplayName = "Left"),
	Center UMETA(DisplayName = "Center"),
	Right UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class EDkInputMode : uint8
{
	GameOnly,
	UIOnly
};

/********/

/* UI Types */
UENUM(BlueprintType)
enum class EConfirmScreenType :uint8
{
	Ok,
	YesOrNo,
	OkOrCancel,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed,
	Cancelled,
	Closed,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault
};

/********/

/* Inventory Types */
UENUM(BlueprintType)
enum class EInventoryItemCategory: uint8
{
	Equipment,
	Consumable,
	CraftingMaterial,
	None
};

UENUM(BlueprintType)
enum class EInventoryGridSlotState: uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};

UENUM(BlueprintType)
enum class EInventoryTileQuadrant: uint8 // 位于背包一个格子的哪个象限
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

/********/
