#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DkInventoryItemFragment.generated.h"


USTRUCT(BlueprintType)
struct DARKKNIGHT_API FInventoryItemFragment
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

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;

public:
	FORCEINLINE FGameplayTag GetFragmentTag() const { return FragmentTag; }
	FORCEINLINE void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }
};

USTRUCT(BlueprintType)
struct FInventoryItemGridFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(const FIntPoint& Size) { GridSize = Size; }
	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float Padding) { GridPadding = Padding; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FIntPoint GridSize{1, 1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float GridPadding = 0.f;
};
