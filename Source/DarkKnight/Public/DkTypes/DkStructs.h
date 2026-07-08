#pragma once
#include "GameplayAbilitySpec.h"
#include "InputAction.h"
#include "DkTypes/DkEnums.h"
#include "StructUtils/InstancedStruct.h"

#include "DkStructs.generated.h"

class UDkInventoryItem;
class ADkPickUpActorBase;
class ADkEquippedActorBase;
class UGameplayAbility;
class UGameplayEffect;
class ADkCharacterBase;
/* UI Structs */
USTRUCT()
struct FOptionsDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	void SetEditConditionFunc(const TFunction<bool()>& InEditConditionFunc)
	{
		EditConditionFunc = InEditConditionFunc;
	}

	bool IsValid() const
	{
		return EditConditionFunc != nullptr;
	}

	bool IsEditableCondition() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}
		return true;
	}

	FString GetDisabledWarningReason() const
	{
		return DisabledWarningReason;
	}

	void SetDisabledWarningReason(const FString& InRichReason)
	{
		DisabledWarningReason = InRichReason;
	}

	bool HasForcedStringValue() const
	{
		return DisabledForcedStringValue.IsSet();
	}

	FString GetDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}

	void SetDisabledForcedStringValue(const FString& InForcedValue)
	{
		DisabledForcedStringValue = InForcedValue;
	}

private:
	TFunction<bool()> EditConditionFunc;
	FString DisabledWarningReason; // 禁用后，设置详情中的提示警告词
	TOptional<FString> DisabledForcedStringValue; // 禁用后，强制切换的StringValue
};

/********/

/* Inventory Structs */
USTRUCT(BlueprintType)
struct FInventoryTileParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FIntPoint TileCoordinate{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 TileIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	EInventoryTileQuadrant TileQuadrant = EInventoryTileQuadrant::None;
};

inline bool operator==(const FInventoryTileParameters& A, const FInventoryTileParameters& B)
{
	return A.TileCoordinate == B.TileCoordinate && A.TileIndex == B.TileIndex && A.TileQuadrant == B.TileQuadrant;
}

USTRUCT()
struct FInventorySpaceQueryResult
{
	GENERATED_BODY()

	bool bHasSpace = false;
	TWeakObjectPtr<UDkInventoryItem> ValidItem = nullptr;
	int32 UpperLeftIndex = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FDkGameplayEffectInfo : public FTableRowBase // TODO:词条生效的时候，Fragment通过对应的属性Tag获取对应的Effect，然后施加给角色
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int GameplayEffectID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Dk.Item.Entry"))
	FGameplayTag GameplayEffectTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> Attribute;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;
};

USTRUCT(BlueprintType)
struct FDkEntryInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName EntryID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Dk.Item.Entry"))
	FGameplayTag GameplayEffectTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bPercent = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;
};

USTRUCT(BlueprintType)
struct FItemEntryInfo
{
	GENERATED_BODY()

	FItemEntryInfo(): MinValue(0.f), MaxValue(0.f)
	{
	}

	FItemEntryInfo(FName InEntryID, float InMinValue, float InMaxValue)
	{
		EntryID = InEntryID;
		MinValue = InMinValue;
		MaxValue = InMaxValue;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName EntryID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxValue;
};

USTRUCT(BlueprintType)
struct FDkItemInfo : public FTableRowBase //项目的整体Item表格
{
	GENERATED_BODY()
	
	// TODO：可以添加校验唯一性函数，在开局等时机调用，用来DT中变量ItemId是否唯一

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemID = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemRequiredLevel = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemQuality ItemQuality;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Dk.Item"))
	FGameplayTag ItemTag; // 用于指示Item之间的不同Tag，程序中需要用Tag来区分Item，而不是ItemID，因此表格中也需要保持唯一性

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EInventoryItemCategory ItemCategory;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bStaticMesh = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UStaticMesh> ItemStaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> ItemSkeletalMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ADkPickUpActorBase> PickUpActorBPClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ADkEquippedActorBase> EquippedActorBPClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName EquippedAttachMeshTag; // 装备所Attach的Mesh的Tag,为空时默认装在MainMesh上

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName EquippedSocketAttachPoint;  // 装备所Attach的SocketName

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ItemPrice = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ItemWeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxStack = INDEX_NONE; // 当MaxStack = INDEX_NONE时，为不可堆叠

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText MainEntry; // 规范：EntryID/MinValue/MaxValue或者EntryID/Value，比如"1001/5/10或者1002/15",词条之间用逗号隔开

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText SubEntry; // 规范：EntryID/MinValue/MaxValue或者EntryID/Value，比如"1001/5/10或者1002/15",词条之间用逗号隔开

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int UniqueEntryID = INDEX_NONE; // 独特词条ID（独特词条可以给角色独特的Ability）

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText UniqueEntry; // 规范：EntryID/MinValue/MaxValue或者EntryID/Value，比如"1001/5/10或者1002/15",词条之间用逗号隔开

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasRandomSubEntry = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MinEntryCount = 1; // 最少随机子词条数

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxEntryCount = 1; // 最多随机子词条数

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MinEntryLevel = 1; // 最低随机子词条等级

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxEntryLevel = 1; // 最高随机子词条等级
};

USTRUCT(BlueprintType)
struct FRewardItemEntry // 奖励物品信息
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemID = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Stack = 0;
};

USTRUCT(BlueprintType)
struct FRewardItemListInfo : public FTableRowBase // TODO: 关卡掉落、死亡掉落等奖励物品信息
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TargetID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FRewardItemEntry> RewardItemList;
};

USTRUCT(BlueprintType)
struct FItemNoticeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(BlueprintReadOnly)
	int ItemCount;
};

/********/

/* Input Structs */
USTRUCT(BlueprintType)
struct FDkInputActionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories = "Dk.Input.Action"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;

	bool IsValid() const
	{
		return InputTag.IsValid() && InputAction;
	}
};

/********/

/* GAS */

USTRUCT(BlueprintType)
struct FHeroBaseStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> HeroClass;

	UPROPERTY(EditAnywhere)
	float Strength = 0.f;

	UPROPERTY(EditAnywhere)
	float Intelligence = 0.f;

	UPROPERTY(EditAnywhere)
	float StrengthGrowthRate = 0.f;

	UPROPERTY(EditAnywhere)
	float IntelligenceGrowthRate = 0.f;

	UPROPERTY(EditAnywhere)
	float BaseMaxHealth = 0.f;

	UPROPERTY(EditAnywhere)
	float BaseMaxEnergy = 0.f;

	UPROPERTY(EditAnywhere)
	float BaseAttackDamage = 0.f;

	UPROPERTY(EditAnywhere)
	float BaseArmor = 0.f;

	UPROPERTY(EditAnywhere)
	float BaseMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	float BaseMoveAcceleration = 2048.f;
};

/********/

/**********************************************************************/
/*                               Dialog                               */
/**********************************************************************/

USTRUCT(BlueprintType)
struct FNpcInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int NpcId;

	UPROPERTY(EditAnywhere)
	FText NpcName;
	
	UPROPERTY(VisibleAnywhere)
	AActor* NpcActor = nullptr;
};

USTRUCT(BlueprintType)
struct FDialogNpcDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FTransform Transform; // 如果MoveType=ENpcMoveType::Teleport的话，移动到的位置

	UPROPERTY(EditAnywhere)
	UAnimMontage* TalkMontage;
	
	UPROPERTY(EditAnywhere)
	ENpcMoveType MoveType;
};

USTRUCT(BlueprintType)
struct FDialogBranchInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText BranchText;

	UPROPERTY(EditAnywhere, meta=(Categories = "Dk.Dialog.Branch"))
	FGameplayTag AddDialogTag; // 用Tag标识这个Branch
	
	UPROPERTY(EditAnywhere, meta=(Categories = "Dk.Dialog.Branch"))
	FGameplayTagContainer Preconditions; // 前置分支Tag标识

	UPROPERTY(EditAnywhere)
	FString TriggerEvent; // 触发事件(这个暂时不知道咋用)

	UPROPERTY(EditAnywhere)
	int JumpToContentId; // 下一步跳转对话Id
};

USTRUCT(BlueprintType)
struct FDialogContent : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Id;

	UPROPERTY(EditAnywhere)
	TMap<int, FDialogNpcDetail> NPCInfos;

	UPROPERTY(EditAnywhere)
	EDialogContentType ContentType;

	UPROPERTY(EditAnywhere)
	TMap<int, FDialogBranchInfo> BranchContents;

	UPROPERTY(EditAnywhere)
	FText ContentText;
	
	UPROPERTY(EditAnywhere)
	int CameraFocusNpcId; // 表示对话camera是以哪一个npc为聚焦的NpcId

	UPROPERTY(EditAnywhere)
	EDialogCameraType CameraType;

	UPROPERTY(EditAnywhere)
	FTransform CustomCameraTransform;
	
	UPROPERTY(EditAnywhere)
	USoundWave* SoundEffect; // 音效

	UPROPERTY(EditAnywhere)
	USoundWave* Dubbing; // 配音
	
	UPROPERTY(EditAnywhere)
	int NextContentId;
	
	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/Script/DarkKnight.CommitTaskDetails"))
	FInstancedStruct CommitTaskDetails;
};

/**********************************************************************/
/*                                Task                                */
/**********************************************************************/

USTRUCT(BlueprintType)
struct FCommitTaskDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int MainTaskId = 0;

	UPROPERTY(EditAnywhere)
	int SubTaskId = 0;

	UPROPERTY(EditAnywhere)
	int CommitCount = 1;
};

USTRUCT(BlueprintType)
struct FCommitTaskDetailsArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FCommitTaskDetails> TaskDetails;
};

USTRUCT(BlueprintType)
struct FEnemyTaskInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Dk.Enemy"))
	FGameplayTag EnemyTag;

	UPROPERTY(EditAnywhere)
	FCommitTaskDetailsArray CommitTaskDetails;
};

USTRUCT(BlueprintType)
struct FSubTaskInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int SubTaskId = 0;

	UPROPERTY(EditAnywhere)
	FText SubTaskDescription;

	UPROPERTY(EditAnywhere)
	int TargetProgress = 0;
	
	UPROPERTY(EditAnywhere, meta=(Categories="Dk.Task.Tracking"))
	FGameplayTag TargetTag;

	UPROPERTY(EditAnywhere)
	int NextSubTaskId;

	UPROPERTY(EditAnywhere)
	TMap<int, int> SubTaskRewards;
};

USTRUCT(BlueprintType)
struct FTaskInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int TaskId;

	UPROPERTY(EditAnywhere)
	FText TaskName;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> TaskIcon;

	UPROPERTY(EditAnywhere)
	FText TaskDescription;
	
	UPROPERTY(EditAnywhere)
	EAreaType TaskAreaType;

	UPROPERTY(EditAnywhere)
	ETaskType TaskType;

	UPROPERTY(EditAnywhere)
	FGameplayTag TaskTag;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer PreconditionTags;
	
	UPROPERTY(EditAnywhere)
	TArray<FSubTaskInfo> SubTaskList;

	UPROPERTY(EditAnywhere)
	TMap<int, int> TaskRewards;
};

USTRUCT(BlueprintType)
struct FSubTaskCompletionStatus
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int SubTaskId;

	UPROPERTY(BlueprintReadOnly)
	ETaskState SubTaskState;

	UPROPERTY(BlueprintReadOnly)
	int CurrentProgress;
};

USTRUCT(BlueprintType)
struct FTaskCompletionStatus
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int TaskId;

	UPROPERTY(BlueprintReadOnly)
	ETaskState TaskState;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FSubTaskCompletionStatus> SubTaskCompletionList;
};

USTRUCT(BlueprintType)
struct FTaskNoticeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	ETaskNoticeState TaskNoticeState;

	UPROPERTY(BlueprintReadOnly)
	FText TaskName;
};

USTRUCT(BlueprintType)
struct FTaskTrackingInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=(Categories="Dk.Task.Tracking"))
	FGameplayTag TaskTrackingTag;
	
	UPROPERTY(EditAnywhere)
	int MainTaskId;

	UPROPERTY(EditAnywhere)
	int SubTaskId;
	
	UPROPERTY(VisibleAnywhere)
	AActor* TrackingActor = nullptr;
};
