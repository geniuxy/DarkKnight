// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryItemManifest.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Object.h"
#include "DkInventoryItem.generated.h"

/**
 * 背包中Item的数据，所有同种Item(占用不同格子)共用一个UDkInventoryItem
 * 但是，ItemManifest中的StackFragment是可以单独使用的
 * 拖拽1个Item时，StackableFragment的StackCount为1；拖拽4个Item时，StackableFragment的StackCount为4
 * StackableFragment可用于DraggedItem、PickUpItem的Manifest表示数量，但在背包里的时候不用StackableFragment表示数量
 * StackableFragment一定可以用来表示MaxStackCount
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	bool IsItemStackable() const;
	int GetItemId() const;
	FText GetItemName() const;
	FGameplayTag GetItemTag() const;
	EInventoryItemCategory GetItemCategory() const;
	int GetItemStackCount() const;
	bool DoesItemTagMatch(const FGameplayTag& ItemTag) const;

	void SetItemManifest(const FInventoryItemManifest& Manifest);
	void SetItemManifest(const FDkItemInfo& ItemInfo, int32 InItemStack);
	const FInventoryItemManifest& GetItemManifest() const { return ItemManifest.Get<FInventoryItemManifest>(); }
	FInventoryItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FInventoryItemManifest>(); }

private:
	UPROPERTY(VisibleAnywhere, meta=(BaseStruct="/Script/DarkKnight.InventoryItemManifest"), Replicated)
	FInstancedStruct ItemManifest;

	UPROPERTY(Replicated)
	int32 TotalStackCount = 0; // 该UDkInventoryItem所有物品的总数量

public:
	LIST_DATA_ACCESSOR(int32, TotalStackCount)
};

template<typename FragmentType>
const FragmentType* GetFragment(const UDkInventoryItem* Item, const FGameplayTag& Tag)
{
	if (!IsValid(Item)) return nullptr;

	const FInventoryItemManifest& Manifest = Item->GetItemManifest();
	return Manifest.GetFragmentOfTypeWithTag<FragmentType>(Tag);
}
