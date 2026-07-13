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
	UIOnly,
	GameAndUI
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

UENUM(BlueprintType)
enum class EActionPriority :uint8
{
	NotApplicable,
	VeryLow,
	Low,
	Medium,
	High,
	VeryHigh,
	Supreme,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None UMETA(DisplayName="None"),
	BasicAttack UMETA(DisplayName="Basic Attack"),
	Aim UMETA(DisplayName="Aim"),
	AbilityOne UMETA(DisplayName="Ability One"),
	AbilityTwo UMETA(DisplayName="Ability Two"),
	AbilityThree UMETA(DisplayName="Ability Three"),
	AbilityFour UMETA(DisplayName="Ability Four"),
	AbilityFive UMETA(DisplayName="Ability Five"),
	AbilitySix UMETA(DisplayName="Ability Six"),
	Confirm UMETA(DisplayName="Confirm"),
	Cancel UMETA(DisplayName="Cancel"),
	Sprint UMETA(DisplayName="Sprint"),
	LockTarget UMETA(DisplayName="LockTarget"),
	SummonMount UMETA(DisplayName="SummonMount")
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

UENUM(BlueprintType)
enum class EWidgetStackType : uint8
{
	Modal,
	GameMenu,
	Interact,
	GameHud,
	Frontend,
	Num UMETA(Hidden)
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

UENUM(BlueprintType)
enum class EItemQuality: uint8 // 位于背包一个格子的哪个象限
{
	Ordinary,
	Rare,
	Exquisite, // 精美的
	Epic,
	UniqueLegend,
	Unknown UMETA(Hidden)
};

/********/

/* Monster Types */
UENUM(BlueprintType)
enum class EMonsterType: uint8
{
	EzMonster,
	Dragon,
	Unknown UMETA(Hidden)
};
/********/

/**********************************************************************/
/*                               Dialog                               */
/**********************************************************************/

UENUM(BlueprintType)
enum class ENpcMoveType: uint8
{
	RelativeLocation UMETA(DisplayName = "移动到相对位置"),
	Teleport UMETA(DisplayName = "瞬间传送"),
	None
};

UENUM(BlueprintType)
enum class EDialogContentType: uint8
{
	Base UMETA(DisplayName = "基本对话"),
	Branch UMETA(DisplayName = "分支对话"),
	None
};

UENUM(BlueprintType)
enum class EDialogCameraType: uint8
{
	CloseUpLeft UMETA(DisplayName = "特写左"),
	CloseShotLeft UMETA(DisplayName = "近景左"),
	MediumShotLeft UMETA(DisplayName = "中景左"),
	DistantViewLeft UMETA(DisplayName = "远景左"),
	CloseUpRight UMETA(DisplayName = "特写右"),
	CloseShotRight UMETA(DisplayName = "近景右"),
	MediumShotRight UMETA(DisplayName = "中景右"),
	DistantViewRight UMETA(DisplayName = "远景右"),
	CustomRelativePosition UMETA(DisplayName = "自定义相对位置"),
	CustomWorldPosition UMETA(DisplayName = "自定义世界位置"),
	None
};

/**********************************************************************/
/*                                Task                                */
/**********************************************************************/

UENUM(BlueprintType)
enum class ETaskType: uint8
{
	MainQuest UMETA(DisplayName = "主线任务"),
	SideQuest UMETA(DisplayName = "支线任务"),
	None
};

UENUM(BlueprintType)
enum class ETaskState: uint8
{
	ToBeAccepted UMETA(DisplayName = "待接"),
	InProgress UMETA(DisplayName = "进行中"),
	Completed UMETA(DisplayName = "已完成"),
	None
};

UENUM(BlueprintType)
enum class ETaskNoticeState: uint8
{
	NewTask UMETA(DisplayName = "新任务"),
	TaskUpdate UMETA(DisplayName = "任务更新"),
	TaskCompleted UMETA(DisplayName = "任务完成"),
	None
};

/**********************************************************************/
/*                                Area                                */
/**********************************************************************/

UENUM(BlueprintType)
enum class EAreaType: uint8
{
	Radiant UMETA(DisplayName = "天辉"),
	Dire UMETA(DisplayName = "夜魇"),
	Hogwarts UMETA(DisplayName = "霍格沃兹"),
	None
};

/**********************************************************************/
/*                               Mount                                */
/**********************************************************************/
UENUM(BlueprintType)
enum class EMountMoveType: uint8
{
	Back UMETA(DisplayName = "后退"),
	Idle UMETA(DisplayName = "停止"),
	Walk UMETA(DisplayName = "慢走"),
	Trot UMETA(DisplayName = "快走"),
	Canter UMETA(DisplayName = "慢跑"),
	Gallop UMETA(DisplayName = "快跑"),
	Sprint UMETA(DisplayName = "冲刺"),
	None
};