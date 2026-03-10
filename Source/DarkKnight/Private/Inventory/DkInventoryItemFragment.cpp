#include "Inventory/DkInventoryItemFragment.h"

#include "DarkKnightDebugHelper.h"
#include "DkTypes/DkStructs.h"
#include "Equipment/DkEquippedActorBase.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Subsytems/DkInventorySubsystem.h"
#include "Widgets/Inventory/Composites/DkInventoryLeaf.h"
#include "Widgets/Inventory/Composites/DkInventoryLeafImage.h"
#include "Widgets/Inventory/Composites/DkInventoryLeafLabeledValue.h"
#include "Widgets/Inventory/Composites/DkInventoryLeafText.h"

void FInventoryItemFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;

	Composite->Expand();
}

bool FInventoryItemFragment::MatchesWidgetTag(const UDkInventoryCompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FInventoryItemImageFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UDkInventoryLeafImage* Image = Cast<UDkInventoryLeafImage>(Composite);
	if (!IsValid(Image)) return;

	Image->SetImage(Icon);
	Image->SetBoxSize(IconDimension);
	Image->SetImageSize(IconDimension);
}

void FInventoryItemTextFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UDkInventoryLeafText* Text = Cast<UDkInventoryLeafText>(Composite);
	if (!IsValid(Text)) return;

	Text->SetText(FragmentText);
}

void FInventoryItemEnumTextFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UDkInventoryLeafText* Text = Cast<UDkInventoryLeafText>(Composite);
	if (!IsValid(Text)) return;

	UEnum* Enum = FindObject<UEnum>(nullptr, *EnumTypePath);
	if (Enum->GetIndexByValue(EnumValue) == INDEX_NONE) return;
	Text->SetText(Enum->GetDisplayNameTextByValue(EnumValue));
}

void FInventoryItemLabeledValueFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UDkInventoryLeafLabeledValue* LabeledValue = Cast<UDkInventoryLeafLabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;

	LabeledValue->SetTextLabel(Text_Label, bCollapseLabel);

	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;

	LabeledValue->SetTextValue(FText::AsNumber(Value, &Options), bCollapseValue);
}

void FInventoryItemLabeledValueFragment::Manifest()
{
	FInventoryItemFragment::Manifest();

	if (bRandomizeOnManifest)
	{
		Value = FMath::FRandRange(Min, Max);
	}
	bRandomizeOnManifest = false;
}

void FInventoryItemEntryFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UDkInventoryLeafText* Text = Cast<UDkInventoryLeafText>(Composite);
	if (!IsValid(Text)) return;

	Text->SetText(GetFormattedText());
}

void FInventoryItemEntryFragment::Manifest()
{
	FInventoryItemFragment::Manifest();

	if (bRandomizeOnManifest)
	{
		Value = FMath::RandRange(Min, Max);
	}
	bRandomizeOnManifest = false;
}

FText FInventoryItemEntryFragment::GetFormattedText() const
{
	FFormatNamedArguments Args;

	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;

	FText FormattedNumber;
	if (bPercent)
	{
		FormattedNumber = FText::AsPercent(Value, &Options);
	}
	else
	{
		FormattedNumber = FText::AsNumber(Value, &Options);
	}

	Args.Add(TEXT("X"), FormattedNumber);

	return FText::Format(Description, Args);
}

void FInventoryItemConsumableFragment::OnConsume(APlayerController* PC)
{
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnConsume(PC);
	}
}

void FInventoryItemConsumableFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	for (const auto& Modifier : ConsumeModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInventoryItemConsumableFragment::Manifest()
{
	FInventoryItemFragment::Manifest();

	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

bool FInventoryItemConsumableFragment::HasOptionalStats() const
{
	bool bHasOptionalStat = false;
	for (const auto& Modifier : ConsumeModifiers)
	{
		const auto& ModRef = Modifier.Get();
		bHasOptionalStat = ModRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_LabeledValue_Stat_0 ||
			ModRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_LabeledValue_Stat_1 ||
			ModRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_LabeledValue_Stat_2;
	}
	return bHasOptionalStat;
}

void FInventoryItemHealthConsumableFragment::OnConsume(APlayerController* PC)
{
	// Get a stats component from the PC or the PC->GetPawn()
	// or get the Ability System Component and apply a Gameplay Effect
	// or call an interface function for Healing()

	Debug::Print(FString::Printf(TEXT("血量相关Item已被使用！恢复量为: %f"), GetValue()));
}

void FInventoryItemManaConsumableFragment::OnConsume(APlayerController* PC)
{
	// Replenish mana however you wish

	Debug::Print(FString::Printf(TEXT("法力值相关Item已被使用！恢复量为: %f"), GetValue()));
}

void FConsumableEntryFragment::OnConsume(APlayerController* PC)
{
	Debug::Print(FString::Printf(TEXT("消耗品类Item已被使用！量为: %f"), GetValue()));
}

void FEquipmentEntryFragment::OnEquip(APlayerController* PC)
{
	Debug::Print(FString::Printf(TEXT("装备！！值为: %f"), GetValue()));
}

void FEquipmentEntryFragment::OnUnEquip(APlayerController* PC)
{
	Debug::Print(FString::Printf(TEXT("卸甲！！值为: %f"), GetValue()));
}

void FInventoryItemEquipmentFragment::OnEquip(APlayerController* PC)
{
	if (bEquipped) return;
	bEquipped = true;
	for (auto& Entry : EquipEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.OnEquip(PC);
	}
}

void FInventoryItemEquipmentFragment::OnUnEquip(APlayerController* PC)
{
	if (!bEquipped) return;
	bEquipped = false;
	for (auto& Entry : EquipEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.OnUnEquip(PC);
	}
}

void FInventoryItemEquipmentFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	for (const auto& Entry : EquipEntries)
	{
		const auto& EntryRef = Entry.Get();
		EntryRef.Assimilate(Composite);
	}
}

void FInventoryItemEquipmentFragment::Manifest()
{
	FInventoryItemFragment::Manifest();

	for (auto& Entry : EquipEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.Manifest();
	}
}

bool FInventoryItemEquipmentFragment::HasOptionalStats() const
{
	bool bHasOptionalStat = false;
	for (const auto& Entry : EquipEntries)
	{
		const auto& EntryRef = Entry.Get();
		bHasOptionalStat = EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_LabeledValue_Stat_0 ||
			EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_LabeledValue_Stat_1 ||
			EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_LabeledValue_Stat_2;
	}
	return bHasOptionalStat;
}

void FInventoryItemEquipmentFragment::UpdateEquipEntries(
	const UObject* WorldContextObject, TArray<FItemEntryInfo> InEntries, bool bMainEntry)
{
	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get(WorldContextObject);
	checkf(InventorySubsystem, TEXT("添加Item词条时，InventorySubsystem为空！"));
	for (int Index = 0; Index < InEntries.Num(); ++Index)
	{
		const FItemEntryInfo& EntryStrInfo = InEntries[Index];
		FDkEntryInfo EntryInfo = InventorySubsystem->GetCachedEntryTable().FindChecked(EntryStrInfo.EntryID);
		FGameplayTag EntryFragmentTag;
		if (bMainEntry)
		{
			EntryFragmentTag = UDkInventoryFunctionLibrary::GetMainEntryTagByIndex(Index);
		}
		else
		{
			EntryFragmentTag = UDkInventoryFunctionLibrary::GetSubEntryTagByIndex(Index);
		}
		FEquipmentEntryFragment NewItemEntryFragment = FEquipmentEntryFragment(
			EntryInfo.Description,
			EntryInfo.GameplayEffectClass,
			EntryStrInfo.MinValue,
			EntryInfo.bPercent,
			EntryFragmentTag
		);
		if (EntryStrInfo.MaxValue != INVALID_INDEX)
		{
			NewItemEntryFragment.SetMaxValue(EntryStrInfo.MaxValue);
		}
		EquipEntries.Add(TInstancedStruct<FEquipmentEntryFragment>::Make(MoveTemp(NewItemEntryFragment)));
	}
}

ADkEquippedActorBase* FInventoryItemEquipmentFragment::SpawnAttachActor(USkeletalMeshComponent* AttachMesh) const
{
	if (!IsValid(EquippedActorClass) || !IsValid(AttachMesh)) return nullptr;

	ADkEquippedActorBase* SpawnedActor = AttachMesh->GetWorld()->SpawnActor<ADkEquippedActorBase>(EquippedActorClass);
	SpawnedActor->AttachToComponent(
		AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketAttachPoint
	);

	return SpawnedActor;
}

void FInventoryItemEquipmentFragment::DestroyAttachActor() const
{
	if (EquippedActor.IsValid())
	{
		EquippedActor->Destroy();
	}
}

void FInventoryItemEquipmentFragment::SetEquippedActor(ADkEquippedActorBase* InEquippedActor)
{
	EquippedActor = InEquippedActor;
}

void FInventoryItemStrengthFragment::OnEquip(APlayerController* PC)
{
	Debug::Print(FString::Printf(TEXT("力量相关Equipment已被使用！增加力量值为: %f"), GetValue()));
}

void FInventoryItemStrengthFragment::OnUnEquip(APlayerController* PC)
{
	Debug::Print(FString::Printf(TEXT("力量相关Equipment已被卸下！减少力量值为: %f"), GetValue()));
}
