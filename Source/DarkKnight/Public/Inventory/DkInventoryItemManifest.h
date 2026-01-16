#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "DkTypes/DkEnums.h"
#include "StructUtils/InstancedStruct.h"
#include "DkInventoryItemManifest.generated.h"

/**
 * ItemManifest 包含创建新 InventoryItem 所需的所有数据。
 */

struct FInventoryItemFragment;
class UDkInventoryItem;

USTRUCT(BlueprintType)
// DARKKNIGHT_API 不加的话，默认只在当前模块可见，别的模块(不在同一个 .build.cs 文件里的代码)就链接不到。
struct DARKKNIGHT_API FInventoryItemManifest
{
	GENERATED_BODY()

	UDkInventoryItem* Manifest(UObject* NewOuter);
	EInventoryItemCategory GetItemCategory() const { return ItemCategory; }

	template<typename T> requires std::derived_from<T, FInventoryItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

private:
	UPROPERTY(EditAnywhere, Category="Inventory",meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FInventoryItemFragment>> Fragments;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	EInventoryItemCategory ItemCategory = EInventoryItemCategory::None;
};

template <typename T> requires std::derived_from<T, FInventoryItemFragment>
const T* FInventoryItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FInventoryItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag))
			{
				continue;
			}

			return FragmentPtr;
		}
	}

	return nullptr;
}
