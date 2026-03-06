#pragma once
#include "ActiveGameplayEffectHandle.h"
#include "InputAction.h"
#include "Inventory/DkInventoryItem.h"

#include "DkStructs.generated.h"

struct FGameplayAbilitySpec;
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
struct FDkItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemID;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemRequiredLevel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemQuality ItemQuality;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "Dk.Item"))
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ADkCharacterBase> EquipmentPlayerBPClass; // 装备对应的角色类

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ADkCharacterBase> EquipmentEnemyBPClass; // 装备对应的敌人类

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemPrice;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ItemWeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Stack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxStack;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MinEntryCount; // 最少词条数

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxEntryCount; // 最多词条数

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MinEntryLevel; // 最低词条等级
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxEntryLevel; // 最高词条等级

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int UniqueEntryID; // 独特词条ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UGameplayEffect>, struct FItemEntryInfo> MainEntry;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UGameplayEffect>, struct FItemEntryInfo> SubEntry;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UGameplayAbility>, FGameplayAbilitySpec> UniqueEntry; // 独特词条可以给角色独特的Ability
};

USTRUCT(BlueprintType)
struct FItemEntryInfo
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
