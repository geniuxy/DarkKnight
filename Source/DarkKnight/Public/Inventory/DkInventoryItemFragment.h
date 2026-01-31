#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "DkGameplayTags.h"
#include "Widgets/Inventory/Composites/DkInventoryCompositeBase.h"
#include "DkInventoryItemFragment.generated.h"


class UDkInventoryCompositeBase;

USTRUCT(BlueprintType)
struct FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemFragment()
	{
	}

	virtual ~FInventoryItemFragment()
	{
	}

	FInventoryItemFragment(const FInventoryItemFragment&) = default;
	FInventoryItemFragment& operator=(const FInventoryItemFragment&) = default;
	FInventoryItemFragment(FInventoryItemFragment&&) = default;
	FInventoryItemFragment& operator=(FInventoryItemFragment&&) = default;

protected:
	UPROPERTY(EditAnywhere, meta = (Categories = "Dk.Inventory.Fragment"), Category="Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, FragmentTag)
};

/*
 * 专门为了同化数据到Widget的Fragment
 */
USTRUCT(BlueprintType)
struct FInventoryItemWidgetFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const;

protected:
	bool MatchesWidgetTag(const UDkInventoryCompositeBase* Composite) const;
};

USTRUCT(BlueprintType)
struct FInventoryItemGridFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemGridFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Grid;
	}

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FIntPoint GridSize{1, 1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float GridPadding = 0.f;

public:
	LIST_DATA_ACCESSOR(FIntPoint, GridSize)
	LIST_DATA_ACCESSOR(float, GridPadding)
};

USTRUCT(BlueprintType)
struct FInventoryItemImageFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemImageFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Icon;
	}

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FVector2D IconDimensions{44.f, 44.f};

public:
	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }
};

USTRUCT(BlueprintType)
struct FInventoryItemStackableFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemStackableFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Stackable;
	}

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 MaxStackSize = 1;

	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 StackCount = 1; // Amount To Fill 需要加到背包里道具的量

public:
	FORCEINLINE int32 GetMaxStackSize() const { return MaxStackSize; }
	LIST_DATA_ACCESSOR(int32, StackCount)
};

/*
 * 消耗品相关的Fragment
 */
USTRUCT(BlueprintType)
struct FInventoryItemConsumableFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) {}
};

USTRUCT(BlueprintType)
struct FInventoryItemHealthConsumableFragment : public FInventoryItemConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Inventory")
	float HealthAmount = 25.f;

	virtual void OnConsume(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FInventoryItemManaConsumableFragment : public FInventoryItemConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Inventory")
	float ManaAmount = 25.f;

	virtual void OnConsume(APlayerController* PC) override;
};
