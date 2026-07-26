#include "Inventory/DkInventoryItemFragment.h"

#include "DarkKnightDebugHelper.h"
#include "DkTypes/DkStructs.h"
#include "Equipment/DkEquippedActorBase.h"
#include "Equipment/DkEquippedActorSkeletal.h"
#include "Equipment/DkEquippedActorStatic.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"
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

void FInventoryItemFragment_Image::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UDkInventoryLeafImage* Image = Cast<UDkInventoryLeafImage>(Composite);
	if (!IsValid(Image)) return;

	Image->SetImage(Icon);
	Image->SetBoxSize(IconDimension);
	Image->SetImageSize(IconDimension);
}

void FInventoryItemFragment_Text::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UDkInventoryLeafText* Text = Cast<UDkInventoryLeafText>(Composite);
	if (!IsValid(Text)) return;

	Text->SetText(FragmentText);
}

void FInventoryItemFragment_EnumText::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UDkInventoryLeafText* Text = Cast<UDkInventoryLeafText>(Composite);
	if (!IsValid(Text)) return;

	UEnum* Enum = FindObject<UEnum>(nullptr, *EnumTypePath);
	if (Enum->GetIndexByValue(EnumValue) == INDEX_NONE) return;
	Text->SetText(Enum->GetDisplayNameTextByValue(EnumValue));
}

void FInventoryItemFragment_LabeledValue::Assimilate(UDkInventoryCompositeBase* Composite) const
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

void FInventoryItemFragment_Entry::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UDkInventoryLeafText* Text = Cast<UDkInventoryLeafText>(Composite);
	if (!IsValid(Text)) return;

	Text->SetText(GetFormattedText());
}

void FInventoryItemFragment_Entry::Manifest()
{
	FInventoryItemFragment::Manifest();

	if (bRandomizeOnManifest)
	{
		Value = FMath::RandRange(Min, Max);
	}
	bRandomizeOnManifest = false;
}

FText FInventoryItemFragment_Entry::GetFormattedText() const
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

void FInventoryItemFragment_Consumable::OnConsume(UAbilitySystemComponent* InASC)
{
	for (auto& Entry : ConsumableEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.OnConsume(InASC);
	}
}

void FInventoryItemFragment_Consumable::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	for (const auto& Entry : ConsumableEntries)
	{
		const auto& EntryRef = Entry.Get();
		EntryRef.Assimilate(Composite);
	}
}

void FInventoryItemFragment_Consumable::Manifest()
{
	FInventoryItemFragment::Manifest();

	for (auto& Entry : ConsumableEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.Manifest();
	}
}

bool FInventoryItemFragment_Consumable::HasOptionalStats() const
{
	bool bHasOptionalStat = false;
	for (const auto& Entry : ConsumableEntries)
	{
		const auto& EntryRef = Entry.Get();
		bHasOptionalStat = EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_0 ||
			EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_1 ||
			EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_2 ||
			EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_3;
		if (bHasOptionalStat)
		{
			break;
		}
	}
	return bHasOptionalStat;
}

void FInventoryItemFragment_Consumable::UpdateConsumableEntries(TArray<FItemEntryInfo> InEntries, bool bMainEntry)
{
	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get();
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
		FEntryFragment_Consumable NewItemEntryFragment = FEntryFragment_Consumable(
			EntryInfo.Description,
			EntryInfo.GameplayEffectClass,
			EntryStrInfo.MinValue,
			EntryInfo.bPercent,
			EntryFragmentTag
		);
		if (EntryStrInfo.MaxValue >= EntryStrInfo.MinValue)
		{
			NewItemEntryFragment.SetMaxValue(EntryStrInfo.MaxValue);
		}
		ConsumableEntries.Add(TInstancedStruct<FEntryFragment_Consumable>::Make(MoveTemp(NewItemEntryFragment)));
	}
}

void FEntryFragment_Consumable::OnConsume(UAbilitySystemComponent* InASC)
{
	Debug::Print(FString::Printf(TEXT("消耗品类Item已被使用！量为: %f"), GetValue()));
}

void FEntryFragment_Equipment::OnEquip(UAbilitySystemComponent* InASC)
{
	Debug::Print(FString::Printf(TEXT("装备！！值为: %f"), GetValue()));
}

void FEntryFragment_Equipment::OnUnEquip(UAbilitySystemComponent* InASC)
{
	Debug::Print(FString::Printf(TEXT("卸甲！！值为: %f"), GetValue()));
}

void FInventoryItemFragment_Equipment::OnEquip(UAbilitySystemComponent* InASC)
{
	if (bEquipped) return;
	bEquipped = true;
	for (auto& Entry : EquipEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.OnEquip(InASC);

		// TODO: 可以把OnEquip的返回变成FActiveGameplayEffectHandle(词条效果)/FGameplayAbilitySpecHandle(能力效果)
		// 然后FInventoryItemFragment_Equipment增加变量TArray<FActiveGameplayEffectHandle/FGameplayAbilitySpecHandle>
		// 就可以在OnUnEquip的时候从ASC中接触这些效果、能力
	}
}

void FInventoryItemFragment_Equipment::OnUnEquip(UAbilitySystemComponent* InASC)
{
	if (!bEquipped) return;
	bEquipped = false;
	for (auto& Entry : EquipEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.OnUnEquip(InASC);
	}
}

void FInventoryItemFragment_Equipment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	for (const auto& Entry : EquipEntries)
	{
		const auto& EntryRef = Entry.Get();
		EntryRef.Assimilate(Composite);
	}
}

void FInventoryItemFragment_Equipment::Manifest()
{
	FInventoryItemFragment::Manifest();

	for (auto& Entry : EquipEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.Manifest();
	}
}

bool FInventoryItemFragment_Equipment::HasOptionalStats() const
{
	bool bHasOptionalStat = false;
	for (const auto& Entry : EquipEntries)
	{
		const auto& EntryRef = Entry.Get();
		bHasOptionalStat = EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_0 ||
			EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_1 ||
			EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_2 ||
			EntryRef.GetFragmentTag() == DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_3;
		if (bHasOptionalStat)
		{
			break;
		}
	}
	return bHasOptionalStat;
}

void FInventoryItemFragment_Equipment::UpdateEquipEntries(TArray<FItemEntryInfo> InEntries, bool bMainEntry)
{
	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get();
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
		FEntryFragment_Equipment NewItemEntryFragment = FEntryFragment_Equipment(
			EntryInfo.Description,
			EntryInfo.GameplayEffectClass,
			EntryStrInfo.MinValue,
			EntryInfo.bPercent,
			EntryFragmentTag
		);
		if (EntryStrInfo.MaxValue >= EntryStrInfo.MinValue)
		{
			NewItemEntryFragment.SetMaxValue(EntryStrInfo.MaxValue);
		}
		EquipEntries.Add(TInstancedStruct<FEntryFragment_Equipment>::Make(MoveTemp(NewItemEntryFragment)));
	}
}

ADkEquippedActorBase* FInventoryItemFragment_Equipment::SpawnAttachActor(USkeletalMeshComponent* AttachMesh) const
{
	if (!IsValid(AttachMesh)) return nullptr;

	checkf(EquippedActorID != INDEX_NONE, TEXT("生成EquipmentItem时，EquippedActorID无效！"))
	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get();
	checkf(InventorySubsystem, TEXT("生成EquipmentItem时，InventorySubsystem为空！"));

	if (!InventorySubsystem->GetCachedItemTable().Contains(EquippedActorID)) return nullptr;
	const FDkItemInfo* EquipmentInfo = InventorySubsystem->GetCachedItemTable().Find(EquippedActorID);
	ADkEquippedActorBase* SpawnedActor =
		AttachMesh->GetWorld()->SpawnActor<ADkEquippedActorBase>(EquipmentInfo->EquippedActorBPClass);
	if (IsValid(SpawnedActor))
	{
		if (EquipmentInfo->bStaticMesh && EquipmentInfo->ItemStaticMesh.IsValid())
		{
			CastChecked<ADkEquippedActorStatic>(SpawnedActor)->SetEquipmentStaticMesh(
				EquipmentInfo->ItemStaticMesh.LoadSynchronous()
			);
		}
		else if (!EquipmentInfo->bStaticMesh && EquipmentInfo->ItemSkeletalMesh.IsValid())
		{
			CastChecked<ADkEquippedActorSkeletal>(SpawnedActor)->SetEquipmentSkeletalMesh(
				EquipmentInfo->ItemSkeletalMesh.LoadSynchronous()
			);
		}
		SpawnedActor->SetEquipmentTag(EquipmentInfo->ItemTag);

		if (!EquipmentInfo->EquippedAttachMeshTag.IsNone() && EquipmentInfo->bStaticMesh)
		{
			AActor* OwnerCharacter = AttachMesh->GetOwner();
			SpawnedActor->AttachToComponent(
				OwnerCharacter->FindComponentByTag<UStaticMeshComponent>(EquipmentInfo->EquippedAttachMeshTag),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				EquipmentInfo->EquippedSocketAttachPoint
			);
		}
		else if (!EquipmentInfo->EquippedAttachMeshTag.IsNone() && !EquipmentInfo->bStaticMesh)
		{
			AActor* OwnerCharacter = AttachMesh->GetOwner();
			SpawnedActor->AttachToComponent(
				OwnerCharacter->FindComponentByTag<USkeletalMeshComponent>(EquipmentInfo->EquippedAttachMeshTag),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				EquipmentInfo->EquippedSocketAttachPoint
			);
		}
		else
		{
			SpawnedActor->AttachToComponent(
				AttachMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				EquipmentInfo->EquippedSocketAttachPoint
			);
		}
	}
	return SpawnedActor;
}
