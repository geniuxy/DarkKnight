#pragma once
#include "DkInventoryItem.h"
#include "DkTypes/DkEnums.h"

#include "DkInventoryCategoryItems.generated.h"

USTRUCT()
struct FInventoryItemBriefInfo // 背包物品简约信息
{
	GENERATED_BODY()

	FInventoryItemBriefInfo()
	{
	}

	FInventoryItemBriefInfo(int InIndex) : Index(InIndex)
	{
	}

	UPROPERTY()
	int Index = INDEX_NONE; // 类别里的第几个

	UPROPERTY()
	UDkInventoryItem* InventoryItem = nullptr;

	UPROPERTY()
	int StackCount = 0;

	bool IsEmpty() const { return InventoryItem == nullptr; }
	bool IsValid() const { return Index != INDEX_NONE; }
	void Empty()
	{
		InventoryItem = nullptr;
		StackCount = 0;
	}
};

USTRUCT()
struct FDkInventorySlotEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	int32 GlobalIndex = INDEX_NONE; // 全局物理坐标（全部视图中的固定格子）
	UPROPERTY()
	EInventoryItemCategory Category = EInventoryItemCategory::None; // 这个格子属于哪个分类
	UPROPERTY()
	FInventoryItemBriefInfo BriefInfo; // 里面的物品 + 分类内序号

	void PreReplicatedRemove(const FFastArraySerializer& InArraySerializer);
	void PostReplicatedAdd(const FFastArraySerializer& InArraySerializer);
	void PostReplicatedChange(const FFastArraySerializer& InArraySerializer);

	bool IsSameItemByTag(FGameplayTag InTag) const;
};

USTRUCT()
struct FDkInventorySlotArray : public FFastArraySerializer
{
	GENERATED_BODY()

	FDkInventorySlotArray() : OwnerComp(nullptr)
	{
	}

	FDkInventorySlotArray(UActorComponent* InOwnerComponent) : OwnerComp(InOwnerComponent)
	{
	}
	
	UPROPERTY()
	TArray<FDkInventorySlotEntry> Slots;

	FDkInventorySlotEntry* FindByGlobalIndex(int32 InGlobalIndex);

	FDkInventorySlotEntry* FindBySlotIndex(EInventoryItemCategory InCategory, int32 InSlotIndex);
	TArray<FDkInventorySlotEntry*> FindByCategory(EInventoryItemCategory InCategory);
    
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FDkInventorySlotEntry, FDkInventorySlotArray>(
			Slots, DeltaParams, *this);
	}

	TWeakObjectPtr<UActorComponent> OwnerComp;
};

template<>
struct TStructOpsTypeTraits<FDkInventorySlotArray> : public TStructOpsTypeTraitsBase2<FDkInventorySlotArray>
{
	enum { WithNetDeltaSerializer = true };
};

USTRUCT(BlueprintType)
struct FInventoryItemCategoryInfo // 背包物品各个类别的相关信息
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EInventoryItemCategory Category;

	UPROPERTY(EditAnywhere)
	UTexture2D* CategoryIcon = nullptr;

	UPROPERTY(EditAnywhere)
	int Rows = 0;

	UPROPERTY(EditAnywhere)
	int Columns = 0;
};

// 这个类的作用是因为ListView的AddItem只接受UObject，AI推荐我创建个UObject来包装
UCLASS()
class DARKKNIGHT_API UInventoryItemCategoryData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FInventoryItemCategoryInfo Info;
};
