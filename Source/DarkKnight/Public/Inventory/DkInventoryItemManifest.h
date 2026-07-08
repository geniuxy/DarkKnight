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

struct FInventoryItemFragment_Entry;
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
	static TArray<FItemEntryInfo> GetItemEntryInfoList(int32 InItemID, const FText& InEntry, bool bMainEntry);
	/********/

	UDkInventoryItem* Manifest(UObject* NewOuter);
	TArray<TInstancedStruct<FItemFragment>>& GetFragmentsMutable() { return Fragments; }

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
		Fragments.Add(TInstancedStruct<FItemFragment>::Make<T>(MoveTemp(Fragment))); // 右移来优化性能
	}

private:
	void ClearFragments();

	// TInstancedStruct 是针对给定 BaseStruct 类型的 类型安全 的 FInstancedStruct 包装器。 
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FItemFragment>> Fragments;

	UPROPERTY(EditAnywhere, Category="Inventory")
	EInventoryItemCategory ItemCategory = EInventoryItemCategory::None;

	UPROPERTY(EditAnywhere, Category="Inventory")
	int32 ItemID = 0;

	UPROPERTY(EditAnywhere, Category="Inventory", meta = (Categories = "Dk.Item"))
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<AActor> PickUpActorClass;  // 用于丢弃Item时使用

public:
	LIST_DATA_ACCESSOR(int, ItemID)
	LIST_DATA_ACCESSOR(EInventoryItemCategory, ItemCategory)
	LIST_DATA_ACCESSOR(FGameplayTag, ItemTag)
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
