#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "DkGameplayTags.h"
#include "StructUtils/InstancedStruct.h"
#include "Widgets/Inventory/Composites/DkInventoryCompositeBase.h"
#include "DkInventoryItemFragment.generated.h"


class ADkEquippedActorBase;
class UDkInventoryCompositeBase;

USTRUCT(BlueprintType)
struct FItemFragment
{
	GENERATED_BODY()

	FItemFragment()
	{
	}

	virtual ~FItemFragment()
	{
	}

	virtual void Manifest() // 显现
	{
	}

	FItemFragment(const FItemFragment&) = default;
	FItemFragment& operator=(const FItemFragment&) = default;
	FItemFragment(FItemFragment&&) = default;
	FItemFragment& operator=(FItemFragment&&) = default;

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
struct FInventoryItemFragment : public FItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const;

protected:
	bool MatchesWidgetTag(const UDkInventoryCompositeBase* Composite) const;
};

USTRUCT(BlueprintType)
struct FInventoryItemGridFragment : public FItemFragment
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

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FVector2D IconDimension{44.f, 44.f};

public:
	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }
};

USTRUCT(BlueprintType)
struct FInventoryItemStackableFragment : public FItemFragment
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

USTRUCT(BlueprintType)
struct FInventoryItemTextFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemTextFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Name;
	}

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FText FragmentText;

public:
	LIST_DATA_ACCESSOR(FText, FragmentText)
};

USTRUCT(BlueprintType)
struct FInventoryItemEnumTextFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemEnumTextFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_ItemType;
		EnumTypePath = "/Script/DarkKnight.EInventoryItemCategory";
		EnumValue = 0;
	}

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FString EnumTypePath;

	UPROPERTY(EditAnywhere, Category="Inventory", Meta = (
		AllowPrivateAccess = "true",
		EditCondition = "EnumTypePath.IsEmpty() == false",
		EditConditionHides
	))
	int32 EnumValue;
};

USTRUCT(BlueprintType)
struct FInventoryItemLabeledValueFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemLabeledValueFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_LabeledValue;
	}

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	virtual void Manifest() override;
	float GetValue() const { return Value; }

	// 第一次出现的时候，该Fragment会随机数值。但是，之后装备或者丢弃，都会保持原有数值
	bool bRandomizeOnManifest{true};

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText Text_Label{};

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float Value{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Min{0};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Max{0};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseLabel{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseValue{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MinFractionalDigits{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxFractionalDigits{1};
};

/*
 *  消耗品 相关的Fragment
 */
USTRUCT(BlueprintType)
struct FInventoryConsumeModifier : public FInventoryItemLabeledValueFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC)
	{
	}
};

USTRUCT(BlueprintType)
struct FInventoryItemConsumableFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemConsumableFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Consumable;
	}

	virtual void OnConsume(APlayerController* PC);
	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	virtual void Manifest() override;
	bool HasOptionalStats() const;

private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FInventoryConsumeModifier>> ConsumeModifiers;
};

USTRUCT(BlueprintType)
struct FInventoryItemHealthConsumableFragment : public FInventoryConsumeModifier
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FInventoryItemManaConsumableFragment : public FInventoryConsumeModifier
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) override;
};

/*
 *  装备 相关的Fragment
 */
USTRUCT(BlueprintType)
struct FInventoryEquipModifier : public FInventoryItemLabeledValueFragment
{
	GENERATED_BODY()

	virtual void OnEquip(APlayerController* PC)
	{
	}

	virtual void OnUnEquip(APlayerController* PC)
	{
	}
};

USTRUCT(BlueprintType)
struct FInventoryItemEquipmentFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemEquipmentFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Equipment;
		bEquipped = false;
	}

	bool bEquipped;
	void OnEquip(APlayerController* PC);
	void OnUnEquip(APlayerController* PC);
	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	virtual void Manifest() override;
	bool HasOptionalStats() const;

	ADkEquippedActorBase* SpawnAttachActor(USkeletalMeshComponent* AttachMesh) const;
	void DestroyAttachActor() const;

	void SetEquippedActor(ADkEquippedActorBase* InEquippedActor);

private:
	UPROPERTY(EditAnywhere, Category="Equipment", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FInventoryEquipModifier>> EquipModifiers;

	UPROPERTY(EditAnywhere, Category="Equipment")
	TSubclassOf<ADkEquippedActorBase> EquippedActorClass;

	TWeakObjectPtr<ADkEquippedActorBase> EquippedActor;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	FName SocketAttachPoint = NAME_None;

	UPROPERTY(EditAnywhere, meta = (Categories = "Dk.Item.Equipment"), Category="Equipment")
	FGameplayTag EquipmentTag = FGameplayTag::EmptyTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, EquipmentTag)
};

USTRUCT(BlueprintType)
struct FInventoryItemStrengthFragment : public FInventoryEquipModifier
{
	GENERATED_BODY()

	virtual void OnEquip(APlayerController* PC) override;
	virtual void OnUnEquip(APlayerController* PC) override;
};