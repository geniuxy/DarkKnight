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

/* CharacterMovement/Action Types */
UENUM(BlueprintType)
enum class ELocomotionStyle :uint8
{
	Walk,
	Run,
	Sprint,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EActionState :uint8
{
	OutOfCombat,
	InCombat,
	InAir,
	CombatFinisherAttacker,
	CombatFinisherReceiver,
	InUIInteractionDialogue,
	Dialogue,
	Death,
	InUIMenu,
	InUIInteractionLoot,
	InUIInteractionMountHorse,
	Riding,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOwnerType :uint8 // ActionComponent的Owner类型
{
	Player,
	NPC,
	Dragon,
	Unknown UMETA(Hidden)
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
	Equipment UMETA(DisplayName = "装备"),
	Consumable UMETA(DisplayName = "消耗品"),
	CraftingMaterial UMETA(DisplayName = "制作材料"),
	None
};

UENUM(BlueprintType)
enum class EInventoryGridSlotState: uint8
{
	Unoccupied,
	Occupied,
	Selected,
	Enabled,
	Disabled,
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

/* Monster Types */
UENUM(BlueprintType)
enum class EMonsterType: uint8
{
	EzMonster,
	Dragon,
	None
};
/********/
