#pragma once

#include "DkStructs.generated.h"

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

/* Inventory Types */
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

/********/
