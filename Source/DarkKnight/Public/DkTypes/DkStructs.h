#pragma once
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpec.h"
#include "InputAction.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkEnums.h"

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
struct FDkGameplayEffectInfo : public FTableRowBase // TODO:看一下这个Struct还是否有必要(估计是没有用了)
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
	FText Description;
};

USTRUCT(BlueprintType)
struct FItemEntryInfo // TODO:看一下这个Struct还是否有必要(估计是没有用了)
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FActiveGameplayEffectHandle GameplayEffectHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int EntryLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int EntryID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText EntryDescription;
};

USTRUCT(BlueprintType)
struct FDkItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemRequiredLevel = INVALID_INDEX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemQuality ItemQuality;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Dk.Item"))
	FGameplayTag ItemTag;

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
	TSubclassOf<ADkCharacterBase> EquipmentPlayerBPClass; // 装备对应的角色类

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ADkCharacterBase> EquipmentEnemyBPClass; // 装备对应的敌人类

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemPrice = INVALID_INDEX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ItemWeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Stack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxStack = INVALID_INDEX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText MainEntry; // 规范：EntryID/MinValue/MaxValue或者EntryID/Value，比如"1001/5/10或者1002/15",词条之间用逗号隔开

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText SubEntry;	// 规范：EntryID/MinValue/MaxValue或者EntryID/Value，比如"1001/5/10或者1002/15",词条之间用逗号隔开
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int UniqueEntryID = INVALID_INDEX; // 独特词条ID（独特词条可以给角色独特的Ability）

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText UniqueEntry;	// 规范：EntryID/MinValue/MaxValue或者EntryID/Value，比如"1001/5/10或者1002/15",词条之间用逗号隔开
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasRandomSubEntry = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MinEntryCount = 1; // 最少随机子词条数

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxEntryCount = 1; // 最多随机子词条数

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MinEntryLevel = 1; // 最低随机子词条等级

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxEntryLevel = 1; // 最高随机子词条等级
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
struct FRewardItemListInfo : public FTableRowBase// TODO: 关卡掉落、死亡掉落等奖励物品信息
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TargetID;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FRewardItemEntry> RewardItemList;
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
