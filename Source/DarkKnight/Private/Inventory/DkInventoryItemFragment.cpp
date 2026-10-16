#include "Inventory/DkInventoryItemFragment.h"

#include "AbilitySystemComponent.h"
#include "DarkKnightDebugHelper.h"
#include "DkTypes/DkStructs.h"
#include "Equipment/DkEquippedActorBase.h"
#include "Equipment/DkEquippedActorSkeletal.h"
#include "Equipment/DkEquippedActorStatic.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
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

FActiveGameplayEffectHandle FInventoryItemFragment_Entry::ApplyEntryEffect(UAbilitySystemComponent* InASC) const
{
	if (!IsValid(InASC))
	{
		return FActiveGameplayEffectHandle();
	}

	// 只有服务端能改属性：客户端自己施加会绕过服务器校验，并且和属性复制打架
	const AActor* OwnerActor = InASC->GetOwner();
	if (!OwnerActor || !OwnerActor->HasAuthority())
	{
		return FActiveGameplayEffectHandle();
	}

	// GE 类不存放在Fragment里，而是用词条Tag去 DT_EntryEffectInfo 查。
	// 这样「哪个词条配哪个GE」只有一处配置，重复配/漏配由 InitializeEntryEffectData 在开局报出来
	const UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get();
	const TSubclassOf<UGameplayEffect> EntryGameplayEffectClass =
		InventorySubsystem ? InventorySubsystem->FindEntryEffectClass(GameplayEffectTag) : nullptr;

	if (!IsValid(EntryGameplayEffectClass))
	{
		// 数据没配全属于内容错误，需要让作者看见，所以直接报出来而不是静默跳过
		Debug::Print(FString::Printf(
			TEXT("词条Tag[%s]在DT_EntryEffectInfo里找不到对应的GameplayEffect，效果无法生效！"),
			GameplayEffectTag.IsValid() ? *GameplayEffectTag.ToString() : TEXT("(空)")
		));
		return FActiveGameplayEffectHandle();
	}

	FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(
		EntryGameplayEffectClass, 1.f, ContextHandle
	);
	if (!SpecHandle.IsValid())
	{
		return FActiveGameplayEffectHandle();
	}

	// 关键一步：把「随机出来的数值」通过 SetByCaller 交给 GE。
	// GE 里对应的 Modifier Magnitude 必须选 SetByCaller 并填同一个 Tag。
	if (GameplayEffectTag.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(GameplayEffectTag, Value);
	}

	return InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
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
			EntryStrInfo.MinValue,
			EntryInfo.bPercent,
			EntryFragmentTag
		);
		// 把 GE 的 SetByCaller 键一起带进 Fragment，施加效果时要用
		NewItemEntryFragment.SetGameplayEffectTag(EntryInfo.GameplayEffectTag);
		if (EntryStrInfo.MaxValue >= EntryStrInfo.MinValue)
		{
			NewItemEntryFragment.SetMaxValue(EntryStrInfo.MaxValue);
		}
		ConsumableEntries.Add(TInstancedStruct<FEntryFragment_Consumable>::Make(MoveTemp(NewItemEntryFragment)));
	}
}

void FEntryFragment_Consumable::OnConsume(UAbilitySystemComponent* InASC)
{
	// 消耗品是 Instant 类效果：施加一次即结束，不需要保存句柄再去移除
	ApplyEntryEffect(InASC);
}

void FEntryFragment_Equipment::Manifest()
{
	FInventoryItemFragment_Entry::Manifest();

	// 句柄是「这个实例此刻是否生效」的运行时状态。
	// Manifest() 代表该 Fragment 被复制到了一个全新的 Item 上（丢弃、交易、重新掉落），
	// 必须从干净状态开始，否则新物品会继承旧句柄：既不会重新施加效果，将来也无法正确移除
	ActiveEffectHandle = FActiveGameplayEffectHandle();
}

void FEntryFragment_Equipment::OnEquip(UAbilitySystemComponent* InASC)
{
	// 幂等：重复装备同一件物品时不要在同一词条上叠两层效果
	if (ActiveEffectHandle.IsValid())
	{
		return;
	}

	ActiveEffectHandle = ApplyEntryEffect(InASC);
}

void FEntryFragment_Equipment::OnUnEquip(UAbilitySystemComponent* InASC)
{
	if (!ActiveEffectHandle.IsValid())
	{
		return;
	}

	// 句柄自己记录了它属于哪个 ASC。优先用它，避免传入的 ASC 和施加时的不是同一个（换 Controller、复活等）
	UAbilitySystemComponent* TargetASC = ActiveEffectHandle.GetOwningAbilitySystemComponent();
	if (!IsValid(TargetASC))
	{
		TargetASC = InASC;
	}

	if (IsValid(TargetASC))
	{
		if (const AActor* OwnerActor = TargetASC->GetOwner(); OwnerActor && OwnerActor->HasAuthority())
		{
			TargetASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
		}
	}

	ActiveEffectHandle = FActiveGameplayEffectHandle();
}

void FInventoryItemFragment_Equipment::OnEquip(UAbilitySystemComponent* InASC)
{
	if (bEquipped) return;
	bEquipped = true;
	for (auto& Entry : EquipEntries)
	{
		auto& EntryRef = Entry.GetMutable();

		// 句柄由每个词条自己保存（而不是在这里统一收集成一个数组）：
		// 词条与句柄一一对应，卸装备时各自移除自己施加的效果；将来要同时管 GE 和 GA 两种句柄也不用改这里的结构
		EntryRef.OnEquip(InASC);
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

	// 复制到新 Item 上时装备状态必须归零。
	// 否则「穿着装备时被丢弃」会产出一件 bEquipped=true 的新物品，它再被装备时会在上面直接 return，词条永远不会生效
	bEquipped = false;

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
			EntryStrInfo.MinValue,
			EntryInfo.bPercent,
			EntryFragmentTag
		);
		// 把 GE 的 SetByCaller 键一起带进 Fragment，施加效果时要用
		NewItemEntryFragment.SetGameplayEffectTag(EntryInfo.GameplayEffectTag);
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
		SpawnedActor->SetItemId(EquippedActorID);
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
