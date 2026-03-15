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
	for (auto& Entry : ConsumableEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.OnConsume(PC);
	}
}

void FInventoryItemConsumableFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	for (const auto& Entry : ConsumableEntries)
	{
		const auto& EntryRef = Entry.Get();
		EntryRef.Assimilate(Composite);
	}
}

void FInventoryItemConsumableFragment::Manifest()
{
	FInventoryItemFragment::Manifest();

	for (auto& Entry : ConsumableEntries)
	{
		auto& EntryRef = Entry.GetMutable();
		EntryRef.Manifest();
	}
}

bool FInventoryItemConsumableFragment::HasOptionalStats() const
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

void FInventoryItemConsumableFragment::UpdateConsumableEntries(
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
		FConsumableEntryFragment NewItemEntryFragment = FConsumableEntryFragment(
			EntryInfo.Description,
			EntryInfo.GameplayEffectClass,
			EntryStrInfo.MinValue,
			EntryInfo.bPercent,
			EntryFragmentTag
		);
		if (EntryStrInfo.MaxValue != INDEX_NONE)
		{
			NewItemEntryFragment.SetMaxValue(EntryStrInfo.MaxValue);
		}
		ConsumableEntries.Add(TInstancedStruct<FConsumableEntryFragment>::Make(MoveTemp(NewItemEntryFragment)));
	}
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
		if (EntryStrInfo.MaxValue != INDEX_NONE)
		{
			NewItemEntryFragment.SetMaxValue(EntryStrInfo.MaxValue);
		}
		EquipEntries.Add(TInstancedStruct<FEquipmentEntryFragment>::Make(MoveTemp(NewItemEntryFragment)));
	}
}

ADkEquippedActorBase* FInventoryItemEquipmentFragment::SpawnAttachActor(USkeletalMeshComponent* AttachMesh) const
{
	if (!IsValid(AttachMesh)) return nullptr;

	checkf(EquippedActorID != INDEX_NONE, TEXT("生成EquipmentItem时，EquippedActorID无效！"))
	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get(AttachMesh);
	checkf(InventorySubsystem, TEXT("生成EquipmentItem时，InventorySubsystem为空！"));

	if (!InventorySubsystem->GetCachedItemTable().Contains(EquippedActorID)) return nullptr;
	const FDkItemInfo* EquipmentInfo = InventorySubsystem->GetCachedItemTable().Find(EquippedActorID);
	ADkEquippedActorBase* SpawnedActor =
		AttachMesh->GetWorld()->SpawnActor<ADkEquippedActorBase>(EquipmentInfo->EquippedActorBPClass);
	if (IsValid(SpawnedActor))
	{
		if (EquipmentInfo->bStaticMesh)
		{
			SpawnedActor->SetEquipmentStaticMesh(EquipmentInfo->ItemStaticMesh);
		}
		else
		{
			SpawnedActor->SetEquipmentSkeletalMesh(EquipmentInfo->ItemSkeletalMesh);
		}
		SpawnedActor->SetEquipmentTag(EquipmentInfo->ItemTag);
		
		SpawnedActor->AttachToComponent(
			AttachMesh,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			EquipmentInfo->EquippedSocketAttachPoint
		);
	}
	return SpawnedActor;
}

ADkEquippedActorBase* FInventoryItemEquipmentFragment::SpawnExtraAttachActor(USkeletalMeshComponent* AttachMesh) const
{
	if (!IsValid(AttachMesh)) return nullptr;
	if (ExtraEquippedActorID == INDEX_NONE) return nullptr;

	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get(AttachMesh);
	checkf(InventorySubsystem, TEXT("生成EquipmentItem时，InventorySubsystem为空！"));
	if (!InventorySubsystem->GetCachedItemTable().Contains(ExtraEquippedActorID)) return nullptr;
	
	const FDkItemInfo* ExtraEquipmentInfo = InventorySubsystem->GetCachedItemTable().Find(ExtraEquippedActorID);
	ADkEquippedActorBase* ExtraSpawnedActor =
		AttachMesh->GetWorld()->SpawnActor<ADkEquippedActorBase>(ExtraEquipmentInfo->EquippedActorBPClass);
	if (IsValid(ExtraSpawnedActor))
	{
		if (ExtraEquipmentInfo->bStaticMesh)
		{
			ExtraSpawnedActor->SetEquipmentStaticMesh(ExtraEquipmentInfo->ItemStaticMesh);
		}
		else
		{
			ExtraSpawnedActor->SetEquipmentSkeletalMesh(ExtraEquipmentInfo->ItemSkeletalMesh);
		}
		ExtraSpawnedActor->SetEquipmentTag(ExtraEquipmentInfo->ItemTag);
		
		ExtraSpawnedActor->AttachToComponent(
			AttachMesh,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			ExtraEquipmentInfo->EquippedSocketAttachPoint
		);
	}
	
	return ExtraSpawnedActor;
}

void FInventoryItemStrengthFragment::OnEquip(APlayerController* PC)
{
	Debug::Print(FString::Printf(TEXT("力量相关Equipment已被使用！增加力量值为: %f"), GetValue()));
}

void FInventoryItemStrengthFragment::OnUnEquip(APlayerController* PC)
{
	Debug::Print(FString::Printf(TEXT("力量相关Equipment已被卸下！减少力量值为: %f"), GetValue()));
}
