#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "DkTypes/DkEnums.h"
#include "DkTypes/DkStructs.h"
#include "Equipment/DkEquippedActorBase.h"
#include "StructUtils/InstancedStruct.h"
#include "DkInventoryItemManifest.generated.h"

/**
 * ItemManifest 包含创建新 InventoryItem 所需的所有数据。
 */

class UDkItemComponent;
class UDkInventoryCompositeBase;
struct FItemFragment;
class UDkInventoryItem;

USTRUCT(BlueprintType)
// DARKKNIGHT_API 不加的话，默认只在当前模块可见，别的模块(不在同一个 .build.cs 文件里的代码)就链接不到。
struct DARKKNIGHT_API FInventoryItemManifest
{
	GENERATED_BODY()

	/* 初始化Fragments */
	void InitializeFragments(const FDkItemInfo* ItemInfo, int32 InItemStack);
	void InitializeItemEntryFragment(int32 InItemID, const FText& InEntry, bool bMainEntry = true);
	void AddItemEntryFragment(
		bool bMainEntry, int32 EntryIndex, int32 InEntryID, int32 InMinValue, int32 InMaxValue = INVALID_INDEX
	);
	static FGameplayTag GetMainEntryTagByIndex(int32 InIndex);
	static FGameplayTag GetSubEntryTagByIndex(int32 InIndex);
	/********/

	UDkInventoryItem* Manifest(UObject* NewOuter);
	TArray<TInstancedStruct<FItemFragment>>& GetFragmentsMutable() { return Fragments; }
	EInventoryItemCategory GetItemCategory() const { return ItemCategory; }
	FGameplayTag GetItemTag() const { return ItemTag; }

	void SpawnPickUpActor(
		const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation
	);

	void AssimilateInventoryFragments(UDkInventoryCompositeBase* Composite) const;

	template <typename T> requires std::derived_from<T, FItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

	template <typename T> requires std::derived_from<T, FItemFragment>
	const T* GetFragmentOfType() const;

	template <typename T> requires std::derived_from<T, FItemFragment>
	T* GetFragmentOfTypeMutable();

	template <typename T> requires std::derived_from<T, FItemFragment>
	TArray<const T*> GetAllFragmentsOfType() const;

	template <typename T> requires std::derived_from<T, FItemFragment>
	void AddFragment(T Fragment)
	{
		Fragments.Add(TInstancedStruct<FItemFragment>::Make<T>(MoveTemp(Fragment))); // 
	}

private:
	void ClearFragments();

	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FItemFragment>> Fragments;

	UPROPERTY(EditAnywhere, Category="Inventory")
	EInventoryItemCategory ItemCategory = EInventoryItemCategory::None;

	UPROPERTY(EditAnywhere, Category="Inventory", meta = (Categories = "Dk.Item"))
	int ItemID = 0;

	UPROPERTY(EditAnywhere, Category="Inventory", meta = (Categories = "Dk.Item"))
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<AActor> PickUpActorClass;

public:
	LIST_DATA_ACCESSOR(int, ItemID)
};

template <typename T> requires std::derived_from<T, FItemFragment>
const T* FInventoryItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FItemFragment>& Fragment : Fragments)
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

template <typename T> requires std::derived_from<T, FItemFragment>
const T* FInventoryItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}

	return nullptr;
}

template <typename T> requires std::derived_from<T, FItemFragment>
T* FInventoryItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}

	return nullptr;
}

template <typename T> requires std::derived_from<T, FItemFragment>
TArray<const T*> FInventoryItemManifest::GetAllFragmentsOfType() const
{
	TArray<const T*> Result;
	for (const TInstancedStruct<FItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			Result.Add(FragmentPtr);
		}
	}

	return Result;
}
