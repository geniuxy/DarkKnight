#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "DkInventoryFastArray.generated.h"


class UDkItemComponent;
class UDkInventoryItem;
class UDkInventoryComponent;

/** Inventory中的单个条目 */
USTRUCT()
struct FDkInventoryFastArrayEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FDkInventoryFastArrayEntry()
	{
	}

private:
	friend struct FDkInventoryFastArray;
	friend UDkInventoryComponent;

	UPROPERTY()
	TObjectPtr<UDkInventoryItem> Item = nullptr;
};

/** Inventory中Items列表 */
USTRUCT(BlueprintType)
struct FDkInventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

	FDkInventoryFastArray() : OwnerComponent(nullptr)
	{
	}

	FDkInventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UDkInventoryItem*> GetAllItems() const;

	//~Begin FFastArraySerializer
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	//~End FFastArraySerializer

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FDkInventoryFastArrayEntry, FDkInventoryFastArray>(
			Entries, DeltaParams, *this);
	}

	UDkInventoryItem* AddEntry(UDkItemComponent* ItemComponent);
	UDkInventoryItem* AddEntry(UDkInventoryItem* Item);
	void RemoveEntry(UDkInventoryItem* Item);

	UDkInventoryItem* FindFirstItemByTag(const FGameplayTag& ItemTag);

private:
	friend UDkInventoryComponent;

	// 复制的Items列表
	UPROPERTY()
	TArray<FDkInventoryFastArrayEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template <>
struct TStructOpsTypeTraits<FDkInventoryFastArray> : public TStructOpsTypeTraitsBase2<FDkInventoryFastArray>
{
	enum { WithNetDeltaSerializer = true };
};
