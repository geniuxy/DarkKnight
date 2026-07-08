#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "DkGameplayTags.h"
#include "StructUtils/InstancedStruct.h"
#include "Widgets/Inventory/Composites/DkInventoryCompositeBase.h"
#include "DkInventoryItemFragment.generated.h"


class UAbilitySystemComponent;
struct FItemEntryInfo;
class UGameplayEffect;
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
 * 专门为了同化数据到Widget(ToolTips)的Fragment
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
struct FItemFragment_Grid : public FItemFragment
{
	GENERATED_BODY()

	FItemFragment_Grid()
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
struct FInventoryItemFragment_Image : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemFragment_Image()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Icon;
	}

	FInventoryItemFragment_Image(UTexture2D* InTexture)
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Icon;
		Icon = InTexture;
	}

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FVector2D IconDimension{90.f, 90.f};

public:
	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }
	FORCEINLINE FVector2D GetIconDimension() const { return IconDimension; }
};

USTRUCT(BlueprintType)
struct FItemFragment_Stackable : public FItemFragment
{
	GENERATED_BODY()

	FItemFragment_Stackable()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Stackable;
	}

	FItemFragment_Stackable(int InStackNum, int InMaxStackNum)
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Stackable;
		StackCount = InStackNum;
		MaxStackSize = InMaxStackNum;
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
struct FInventoryItemFragment_Text : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemFragment_Text()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Name;
	}

	FInventoryItemFragment_Text(FText InText, FGameplayTag InTag = DkGameplayTags::Dk_Inventory_Fragment_Name)
	{
		FragmentTag = InTag;
		FragmentText = InText;
	}

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FText FragmentText;

public:
	LIST_DATA_ACCESSOR(FText, FragmentText)
};

USTRUCT(BlueprintType)
struct FInventoryItemFragment_EnumText : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemFragment_EnumText()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_ItemType;
		EnumTypePath = "/Script/DarkKnight.EInventoryItemCategory";
		EnumValue = 0;
	}

	FInventoryItemFragment_EnumText(
		int InEnumValue,
		FString InEnumTypePath = "/Script/DarkKnight.EInventoryItemCategory",
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_ItemType)
	{
		FragmentTag = InFragmentTag;
		EnumTypePath = InEnumTypePath;
		EnumValue = InEnumValue;
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
struct FInventoryItemFragment_LabeledValue : public FInventoryItemFragment // 用于售价、等级要求等key:Value形式的Fragment
{
	GENERATED_BODY()

	FInventoryItemFragment_LabeledValue()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_LabeledValue;
	}

	FInventoryItemFragment_LabeledValue(FGameplayTag InFragmentTag)
	{
		FragmentTag = InFragmentTag;
	}

	FInventoryItemFragment_LabeledValue(
		const FText& InText,
		float InValue,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_LabeledValue)
	{
		FragmentTag = InFragmentTag;
		Text_Label = InText;
		Value = InValue;
	}

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	float GetValue() const { return Value; }
	void SetValue(int32 InValue) { Value = InValue; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText Text_Label{};

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float Value{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseLabel{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseValue{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MinFractionalDigits{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxFractionalDigits{1};
};

USTRUCT(BlueprintType)
struct FInventoryItemFragment_Entry : public FInventoryItemFragment // 用于词条描述、随机数值并生效的Fragment
{
	GENERATED_BODY()

	FInventoryItemFragment_Entry()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry;
	}

	FInventoryItemFragment_Entry(FGameplayTag InFragmentTag)
	{
		FragmentTag = InFragmentTag;
	}

	FInventoryItemFragment_Entry(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
	{
		FragmentTag = InFragmentTag;
		Description = InText;
		EntryGameplayEffectClass = InGameplayEffectClass;
		Min = InValue;
		Max = InValue;
		bPercent = bInPercent;
	}

	FInventoryItemFragment_Entry(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InMaxValue,
		float InMinValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
	{
		FragmentTag = InFragmentTag;
		Description = InText;
		EntryGameplayEffectClass = InGameplayEffectClass;
		Min = InMinValue;
		Max = InMaxValue;
		bPercent = bInPercent;
	}

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	virtual void Manifest() override;
	float GetValue() const { return Value; }
	FText GetFormattedText() const;
	void SetMinValue(float InValue) { Min = InValue; }
	void SetMaxValue(float InValue) { Max = InValue; }
	void SetIsPercent(bool bInPercent) { bPercent = bInPercent; }

	// 第一次出现的时候，该Fragment会随机数值。但是，之后装备或者丢弃，都会保持原有数值
	bool bRandomizeOnManifest{true};

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText Description{};

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float Value{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Min{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Max{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bPercent{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MinFractionalDigits{0};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxFractionalDigits{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UGameplayEffect> EntryGameplayEffectClass;
};

/*
 *  消耗品 相关的Fragment
 */
USTRUCT(BlueprintType)
struct FEntryFragment_Consumable : public FInventoryItemFragment_Entry // 用于消耗品类型的词条描述、随机数值并生效的Fragment
{
	GENERATED_BODY()

	FEntryFragment_Consumable() : FInventoryItemFragment_Entry()
	{
	}

	FEntryFragment_Consumable(FGameplayTag InFragmentTag)
		: FInventoryItemFragment_Entry(InFragmentTag)
	{
	}

	FEntryFragment_Consumable(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
		: FInventoryItemFragment_Entry(InText, InGameplayEffectClass, InValue, bInPercent, InFragmentTag)
	{
	}

	FEntryFragment_Consumable(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InMaxValue,
		float InMinValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
		: FInventoryItemFragment_Entry(InText, InGameplayEffectClass, InMaxValue, InMinValue, bInPercent, InFragmentTag)
	{
	}

	void OnConsume(UAbilitySystemComponent* InASC);
};

USTRUCT(BlueprintType)
struct FInventoryItemFragment_Consumable : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemFragment_Consumable()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Consumable;
	}

	FInventoryItemFragment_Consumable(FGameplayTag InFragmentTag)
	{
		FragmentTag = InFragmentTag;
	}

	virtual void OnConsume(UAbilitySystemComponent* InASC);
	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	virtual void Manifest() override;
	bool HasOptionalStats() const;

	void UpdateConsumableEntries(TArray<FItemEntryInfo> InEntries, bool bMainEntry);

private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FEntryFragment_Consumable>> ConsumableEntries;
};

/*
 *  装备 相关的Fragment
 */
USTRUCT(BlueprintType)
struct FEntryFragment_Equipment : public FInventoryItemFragment_Entry // 用于装备类型的词条描述、随机数值并生效的Fragment
{
	GENERATED_BODY()

	FEntryFragment_Equipment() : FInventoryItemFragment_Entry()
	{
	}

	FEntryFragment_Equipment(FGameplayTag InFragmentTag)
		: FInventoryItemFragment_Entry(InFragmentTag)
	{
	}

	FEntryFragment_Equipment(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
		: FInventoryItemFragment_Entry(InText, InGameplayEffectClass, InValue, bInPercent, InFragmentTag)
	{
	}

	FEntryFragment_Equipment(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InMaxValue,
		float InMinValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
		: FInventoryItemFragment_Entry(InText, InGameplayEffectClass, InMaxValue, InMinValue, bInPercent, InFragmentTag)
	{
	}

	void OnEquip(UAbilitySystemComponent* InASC);
	void OnUnEquip(UAbilitySystemComponent* InASC);
};

USTRUCT(BlueprintType)
struct FInventoryItemFragment_Equipment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemFragment_Equipment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Equipment;
	}

	FInventoryItemFragment_Equipment(
		int32 InEquippedActorID,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Equipment)
	{
		FragmentTag = InFragmentTag;
		EquippedActorID = InEquippedActorID;
	}

	bool bEquipped{false};
	void OnEquip(UAbilitySystemComponent* InASC);
	void OnUnEquip(UAbilitySystemComponent* InASC);
	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	virtual void Manifest() override;
	bool HasOptionalStats() const;

	void UpdateEquipEntries(TArray<FItemEntryInfo> InEntries, bool bMainEntry);

	ADkEquippedActorBase* SpawnAttachActor(USkeletalMeshComponent* AttachMesh) const;

private:
	UPROPERTY(EditAnywhere, Category="Equipment", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FEntryFragment_Equipment>> EquipEntries;

	UPROPERTY(EditAnywhere, Category="Equipment")
	int32 EquippedActorID = INDEX_NONE;

	FGameplayTag EquippedActorTag = FGameplayTag::EmptyTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, EquippedActorTag)
};
