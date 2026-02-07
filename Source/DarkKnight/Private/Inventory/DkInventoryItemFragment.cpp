#include "Inventory/DkInventoryItemFragment.h"

#include "DarkKnightDebugHelper.h"
#include "Equipment/DkEquipActorBase.h"
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

void FInventoryItemEquipmentFragment::OnEquip(APlayerController* PC)
{
	if (bEquipped) return;
	bEquipped = true;
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnEquip(PC);
	}
}

void FInventoryItemEquipmentFragment::OnUnEquip(APlayerController* PC)
{
	if (!bEquipped) return;
	bEquipped = false;
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnUnEquip(PC);
	}
}

void FInventoryItemEquipmentFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	for (const auto& Modifier : EquipModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInventoryItemEquipmentFragment::Manifest()
{
	FInventoryItemFragment::Manifest();

	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

bool FInventoryItemEquipmentFragment::HasOptionalStats() const
{
	bool bHasOptionalStat = false;
	for (const auto& Modifier : EquipModifiers)
	{
		const auto& ModRef = Modifier.Get();
		bHasOptionalStat = ModRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_LabeledValue_Stat_0 ||
			ModRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_LabeledValue_Stat_1 ||
			ModRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_LabeledValue_Stat_2;
	}
	return bHasOptionalStat;
}

ADkEquipActorBase* FInventoryItemEquipmentFragment::SpawnAttachActor(USkeletalMeshComponent* AttachMesh) const
{
	if (!IsValid(EquipActorClass) || !IsValid(AttachMesh)) return nullptr;

	ADkEquipActorBase* SpawnedActor = AttachMesh->GetWorld()->SpawnActor<ADkEquipActorBase>(EquipActorClass);
	SpawnedActor->AttachToComponent(
		AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketAttachPoint
	);

	return SpawnedActor;
}

void FInventoryItemEquipmentFragment::DestroyAttachActor() const
{
	if (EquipActor.IsValid())
	{
		EquipActor->Destroy();
	}
}

void FInventoryItemStrengthFragment::OnEquip(APlayerController* PC)
{
	Debug::Print(FString::Printf(TEXT("力量相关Equipment已被使用！增加力量值为: %f"), GetValue()));
}

void FInventoryItemStrengthFragment::OnUnEquip(APlayerController* PC)
{
	Debug::Print(FString::Printf(TEXT("力量相关Equipment已被卸下！减少力量值为: %f"), GetValue()));
}
