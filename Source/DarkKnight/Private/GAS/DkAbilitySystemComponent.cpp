// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DkAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DkTypes/DkEnums.h"
#include "DkGameplayTags.h"
#include "DataAssets/PA_AbilitySystemGenerics.h"
#include "GameplayEffectExtension.h"
#include "DkTypes/DkStructs.h"
#include "GAS/DkAttributeSet.h"
#include "GAS/DkHeroAttributeSet.h"
#include "Net/UnrealNetwork.h"

UDkAbilitySystemComponent::UDkAbilitySystemComponent()
{
	GetGameplayAttributeValueChangeDelegate(UDkAttributeSet::GetHealthAttribute()).AddUObject(
		this, &ThisClass::HealthUpdated
	);
	GetGameplayAttributeValueChangeDelegate(UDkAttributeSet::GetEnergyAttribute()).AddUObject(
		this, &ThisClass::EnergyUpdated
	);

	GenericConfirmInputID = (int32)EAbilityInputID::Confirm;
	GenericCancelInputID = (int32)EAbilityInputID::Cancel;
}

void UDkAbilitySystemComponent::ServerSideInit()
{
	InitializeBaseAttributes();
	InitializeBaseGameplayEffects();
	GiveInitialAbilities();
}

void UDkAbilitySystemComponent::ApplyFullStatsEffect()
{
	if (AbilitySystemGenerics && AbilitySystemGenerics->GetFullStatsEffect())
	{
		AuthApplyGameplayEffect(AbilitySystemGenerics->GetFullStatsEffect());
	}
}

void UDkAbilitySystemComponent::AuthApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int Level)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(GameplayEffect, Level, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

UInputMappingContext* UDkAbilitySystemComponent::GetInputMappingContext() const
{
	return AbilitySystemGenerics->GetGameplayInputMappingContext();
}

TMap<EAbilityInputID, UInputAction*> UDkAbilitySystemComponent::GetGameplayAbilityInputActions() const
{
	return AbilitySystemGenerics->GetGameplayAbilityInputActions();
}

void UDkAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDkAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDkAbilitySystemComponent, LockTarget);
}

void UDkAbilitySystemComponent::InitializeBaseAttributes()
{
	if (!AbilitySystemGenerics || !AbilitySystemGenerics->GetBaseStatDataTable() ||
		!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	UDataTable* BaseStatDataTable = AbilitySystemGenerics->GetBaseStatDataTable();
	const FHeroBaseStats* BaseStats = nullptr;
	for (const TPair<FName, uint8*>& DataPair : BaseStatDataTable->GetRowMap())
	{
		BaseStats = BaseStatDataTable->FindRow<FHeroBaseStats>(DataPair.Key, "");
		if (BaseStats && BaseStats->HeroClass == GetOwner()->GetClass())
		{
			break;
		}
	}

	if (BaseStats)
	{
		SetNumericAttributeBase(UDkAttributeSet::GetMaxHealthAttribute(), BaseStats->BaseMaxHealth);
		SetNumericAttributeBase(UDkAttributeSet::GetMaxEnergyAttribute(), BaseStats->BaseMaxEnergy);
		SetNumericAttributeBase(UDkAttributeSet::GetAttackDamageAttribute(), BaseStats->BaseAttackDamage);
		SetNumericAttributeBase(UDkAttributeSet::GetArmorAttribute(), BaseStats->BaseArmor);
		SetNumericAttributeBase(UDkAttributeSet::GetMoveSpeedAttribute(), BaseStats->BaseMoveSpeed);
		SetNumericAttributeBase(UDkAttributeSet::GetMoveAccelerationAttribute(), BaseStats->BaseMoveAcceleration);

		SetNumericAttributeBase(UDkHeroAttributeSet::GetStrengthAttribute(), BaseStats->Strength);
		SetNumericAttributeBase(UDkHeroAttributeSet::GetStrengthGrowthRateAttribute(), BaseStats->StrengthGrowthRate);
		SetNumericAttributeBase(UDkHeroAttributeSet::GetIntelligenceAttribute(), BaseStats->Intelligence);
		SetNumericAttributeBase(
			UDkHeroAttributeSet::GetIntelligenceGrowthRateAttribute(), BaseStats->IntelligenceGrowthRate
		);
	}
}

void UDkAbilitySystemComponent::InitializeBaseGameplayEffects()
{
	if (!AbilitySystemGenerics || !GetOwner() || !GetOwner()->HasAuthority()) return;

	for (const TSubclassOf<UGameplayEffect>& EffectClass : AbilitySystemGenerics->GetInitialGameplayEffects())
	{
		AuthApplyGameplayEffect(EffectClass);
	}
}

void UDkAbilitySystemComponent::GiveInitialAbilities()
{
	if (!AbilitySystemGenerics || !GetOwner() || !GetOwner()->HasAuthority()) return;

	for (const TPair<EAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : Abilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 0, (int32)AbilityPair.Key, nullptr));
	}

	for (const TPair<EAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : BasicAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 1, (int32)AbilityPair.Key, nullptr));
	}

	for (const TSubclassOf<UGameplayAbility>& PassiveAbility : AbilitySystemGenerics->GetPassiveAbilities())
	{
		GiveAbility(FGameplayAbilitySpec(PassiveAbility, 1, -1, nullptr));
	}

	for (const TPair<EAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : AbilitySystemGenerics->GetCommonAbilities())
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 1, (int32)AbilityPair.Key, nullptr));
	}
}

void UDkAbilitySystemComponent::HealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (!AbilitySystemGenerics || !GetOwner() || !GetOwner()->HasAuthority()) return;

	bool bFound = false;
	float MaxHealth = GetGameplayAttributeValue(UDkAttributeSet::GetMaxHealthAttribute(), bFound);
	if (bFound && ChangeData.NewValue >= MaxHealth)
	{
		if (!HasMatchingGameplayTag(DkGameplayTags::Dk_Stats_Health_Full))
		{
			// 只在本地执行Tag添加
			AddLooseGameplayTag(DkGameplayTags::Dk_Stats_Health_Full);
		}
	}
	else
	{
		RemoveLooseGameplayTag(DkGameplayTags::Dk_Stats_Health_Full);
	}

	if (ChangeData.NewValue <= 0.f)
	{
		if (!HasMatchingGameplayTag(DkGameplayTags::Dk_Stats_Health_Empty))
		{
			AddLooseGameplayTag(DkGameplayTags::Dk_Stats_Health_Empty);

			// 这一步给角色施加了Dk_Stats_Dead的Tag
			if (AbilitySystemGenerics->GetDeathEffect())
			{
				AuthApplyGameplayEffect(AbilitySystemGenerics->GetDeathEffect());
			}

			FGameplayEventData DeadAbilityEventData;
			if (ChangeData.GEModData)
			{
				DeadAbilityEventData.ContextHandle = ChangeData.GEModData->EffectSpec.GetContext();
			}

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				GetOwner(), DkGameplayTags::Dk_Ability_Passive_Dead_Event_Activate, DeadAbilityEventData
			);
		}
	}
	else
	{
		RemoveLooseGameplayTag(DkGameplayTags::Dk_Stats_Health_Empty);
	}
}

void UDkAbilitySystemComponent::EnergyUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (!AbilitySystemGenerics || !GetOwner() || !GetOwner()->HasAuthority()) return;

	bool bFound = false;
	float MaxMana = GetGameplayAttributeValue(UDkAttributeSet::GetMaxEnergyAttribute(), bFound);
	if (bFound && ChangeData.NewValue >= MaxMana)
	{
		if (!HasMatchingGameplayTag(DkGameplayTags::Dk_Stats_Energy_Full))
		{
			// 只在本地执行Tag添加
			AddLooseGameplayTag(DkGameplayTags::Dk_Stats_Energy_Full);
		}
	}
	else
	{
		RemoveLooseGameplayTag(DkGameplayTags::Dk_Stats_Energy_Full);
	}

	if (ChangeData.NewValue <= 0.f)
	{
		if (!HasMatchingGameplayTag(DkGameplayTags::Dk_Stats_Energy_Empty))
		{
			AddLooseGameplayTag(DkGameplayTags::Dk_Stats_Energy_Empty);
		}
	}
	else
	{
		RemoveLooseGameplayTag(DkGameplayTags::Dk_Stats_Energy_Empty);
	}
}
