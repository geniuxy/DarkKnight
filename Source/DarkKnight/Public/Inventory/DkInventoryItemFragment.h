#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "DkGameplayTags.h"
#include "StructUtils/InstancedStruct.h"
#include "Widgets/Inventory/Composites/DkInventoryCompositeBase.h"
#include "DkInventoryItemFragment.generated.h"


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

	FInventoryItemImageFragment(UTexture2D* InTexture)
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
struct FInventoryItemStackableFragment : public FItemFragment
{
	GENERATED_BODY()

	FInventoryItemStackableFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Stackable;
	}

	FInventoryItemStackableFragment(int InStackNum, int InMaxStackNum)
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
struct FInventoryItemTextFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemTextFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Name;
	}

	FInventoryItemTextFragment(FText InText, FGameplayTag InTag = DkGameplayTags::Dk_Inventory_Fragment_Name)
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
struct FInventoryItemEnumTextFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemEnumTextFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_ItemType;
		EnumTypePath = "/Script/DarkKnight.EInventoryItemCategory";
		EnumValue = 0;
	}

	FInventoryItemEnumTextFragment(
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
struct FInventoryItemLabeledValueFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemLabeledValueFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_LabeledValue;
	}

	FInventoryItemLabeledValueFragment(FGameplayTag InFragmentTag)
	{
		FragmentTag = InFragmentTag;
	}

	FInventoryItemLabeledValueFragment(
		const FText& InText,
		float InValue,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_LabeledValue)
	{
		FragmentTag = InFragmentTag;
		Text_Label = InText;
		Min = InValue;
		Max = InValue;
	}

	FInventoryItemLabeledValueFragment(
		const FText& InText,
		float InMaxValue,
		float InMinValue,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_LabeledValue)
	{
		FragmentTag = InFragmentTag;
		Text_Label = InText;
		Min = InMinValue;
		Max = InMaxValue;
	}

	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	virtual void Manifest() override;
	float GetValue() const { return Value; }
	void SetMinValue(int32 InValue) { Min = InValue; }
	void SetMaxValue(int32 InValue) { Max = InValue; }

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

USTRUCT(BlueprintType)
struct FInventoryItemEntryFragment : public FInventoryItemFragment // 用于词条描述、随机数值并生效的Fragment
{
	GENERATED_BODY()

	FInventoryItemEntryFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry;
	}

	FInventoryItemEntryFragment(FGameplayTag InFragmentTag)
	{
		FragmentTag = InFragmentTag;
	}

	FInventoryItemEntryFragment(
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

	FInventoryItemEntryFragment(
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
struct FInventoryConsumeModifier : public FInventoryItemLabeledValueFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC)
	{
	}
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

USTRUCT(BlueprintType)
struct FConsumableEntryFragment : public FInventoryItemEntryFragment // 用于消耗品类型的词条描述、随机数值并生效的Fragment
{
	GENERATED_BODY()

	FConsumableEntryFragment() : FInventoryItemEntryFragment()
	{
	}

	FConsumableEntryFragment(FGameplayTag InFragmentTag)
		: FInventoryItemEntryFragment(InFragmentTag)
	{
	}

	FConsumableEntryFragment(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
		: FInventoryItemEntryFragment(InText, InGameplayEffectClass, InValue, bInPercent, InFragmentTag)
	{
	}

	FConsumableEntryFragment(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InMaxValue,
		float InMinValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
		: FInventoryItemEntryFragment(InText, InGameplayEffectClass, InMaxValue, InMinValue, bInPercent, InFragmentTag)
	{
	}

	void OnConsume(APlayerController* PC);
};

USTRUCT(BlueprintType)
struct FInventoryItemConsumableFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemConsumableFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Consumable;
	}

	FInventoryItemConsumableFragment(FGameplayTag InFragmentTag)
	{
		FragmentTag = InFragmentTag;
	}

	virtual void OnConsume(APlayerController* PC);
	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	virtual void Manifest() override;
	bool HasOptionalStats() const;

	void UpdateConsumableEntries(const UObject* WorldContextObject, TArray<FItemEntryInfo> InEntries, bool bMainEntry);

private:
	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FConsumableEntryFragment>> ConsumableEntries;
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
struct FEquipmentEntryFragment : public FInventoryItemEntryFragment // 用于装备类型的词条描述、随机数值并生效的Fragment
{
	GENERATED_BODY()

	FEquipmentEntryFragment() : FInventoryItemEntryFragment()
	{
	}

	FEquipmentEntryFragment(FGameplayTag InFragmentTag)
		: FInventoryItemEntryFragment(InFragmentTag)
	{
	}

	FEquipmentEntryFragment(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
		: FInventoryItemEntryFragment(InText, InGameplayEffectClass, InValue, bInPercent, InFragmentTag)
	{
	}

	FEquipmentEntryFragment(
		const FText& InText,
		const TSubclassOf<UGameplayEffect>& InGameplayEffectClass,
		float InMaxValue,
		float InMinValue,
		bool bInPercent = false,
		FGameplayTag InFragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Entry)
		: FInventoryItemEntryFragment(InText, InGameplayEffectClass, InMaxValue, InMinValue, bInPercent, InFragmentTag)
	{
	}

	void OnEquip(APlayerController* PC);
	void OnUnEquip(APlayerController* PC);
};

USTRUCT(BlueprintType)
struct FInventoryItemEquipmentFragment : public FInventoryItemFragment
{
	GENERATED_BODY()

	FInventoryItemEquipmentFragment()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Equipment;
	}

	FInventoryItemEquipmentFragment(
		const TSubclassOf<ADkEquippedActorBase>& InEquippedActorClass,
		FGameplayTag InEquipmentTag = FGameplayTag::EmptyTag,
		FName InSocketAttachPoint = NAME_None)
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Equipment;
		EquippedActorClass = InEquippedActorClass;
		EquipmentTag = InEquipmentTag;
		SocketAttachPoint = InSocketAttachPoint;
	}

	bool bEquipped{false};
	void OnEquip(APlayerController* PC);
	void OnUnEquip(APlayerController* PC);
	virtual void Assimilate(UDkInventoryCompositeBase* Composite) const override;
	virtual void Manifest() override;
	bool HasOptionalStats() const;

	void UpdateEquipEntries(const UObject* WorldContextObject, TArray<FItemEntryInfo> InEntries, bool bMainEntry);

	ADkEquippedActorBase* SpawnAttachActor(int32 EquipmentID, USkeletalMeshComponent* AttachMesh) const;
	void DestroyAttachActor() const;

	void SetEquippedActor(ADkEquippedActorBase* InEquippedActor);

private:
	UPROPERTY(EditAnywhere, Category="Equipment", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FEquipmentEntryFragment>> EquipEntries;

	UPROPERTY(EditAnywhere, Category="Equipment")
	TSubclassOf<ADkEquippedActorBase> EquippedActorClass;

	UPROPERTY(EditAnywhere, Category="Equipment")
	FName SocketAttachPoint = NAME_None;

	UPROPERTY(EditAnywhere, meta = (Categories = "Dk.Item.Equipment"), Category="Equipment")
	FGameplayTag EquipmentTag = FGameplayTag::EmptyTag;

	TWeakObjectPtr<ADkEquippedActorBase> EquippedActor;

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
