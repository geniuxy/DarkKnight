#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
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
	UPROPERTY(EditAnywhere, meta = (Categories = "Dk.Inventory.Fragment"), Category="Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, FragmentTag)
};

USTRUCT(BlueprintType)
struct FInventoryItemGridFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

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

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FVector2D IconDimensions{44.f, 44.f};

public:
	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }
};
