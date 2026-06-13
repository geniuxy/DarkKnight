// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_BasicAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "DkGameplayTags.h"
#include "DarkKnightDebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

UGA_BasicAttack::UGA_BasicAttack()
{
	AbilityTags.AddTag(DkGameplayTags::Dk_Ability_BasicAttack_Tag);
	ActivationRequiredTags.AddTag(DkGameplayTags::Dk_Stats_Equipped_PrimaryWeapon);
	ActivationRequiredTags.AddTag(DkGameplayTags::Dk_Stats_CanAttack);
	BlockAbilitiesWithTag.AddTag(DkGameplayTags::Dk_Ability_BasicAttack_Tag);
}

void UGA_BasicAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayBasicAttackMontage =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, BasicAttackMontage);
		PlayBasicAttackMontage->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayBasicAttackMontage->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayBasicAttackMontage->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayBasicAttackMontage->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayBasicAttackMontage->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitComboChangeEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, DkGameplayTags::Dk_Ability_BasicAttack_Event_Change, nullptr, false, false
		);
		WaitComboChangeEvent->EventReceived.AddDynamic(this, &ThisClass::ComboChangedEventReceived);
		WaitComboChangeEvent->ReadyForActivation();
	}

	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitDamageEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, DkGameplayTags::Dk_Ability_BasicAttack_Event_Damage, nullptr, false, false
		);
		WaitDamageEvent->EventReceived.AddDynamic(this, &ThisClass::DoDamage);
		WaitDamageEvent->ReadyForActivation();
	}

	NextComboName = NAME_None;
	SetupWaitComboInputPress();
}

void UGA_BasicAttack::SetupWaitComboInputPress()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &ThisClass::HandleInputPress);
	WaitInputPress->ReadyForActivation();
}

void UGA_BasicAttack::HandleInputPress(float TimeWaited)
{
	SetupWaitComboInputPress();
	TryCommitCombo();
}

void UGA_BasicAttack::TryCommitCombo()
{
	if (NextComboName == NAME_None)
	{
		return;
	}

	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();
	if (!OwnerAnimInstance)
	{
		return;
	}

	OwnerAnimInstance->Montage_SetNextSection(
		OwnerAnimInstance->Montage_GetCurrentSection(BasicAttackMontage),
		NextComboName,
		BasicAttackMontage
	);
}

void UGA_BasicAttack::ComboChangedEventReceived(FGameplayEventData Data)
{
	FGameplayTag EventTag = Data.EventTag;

	if (EventTag == DkGameplayTags::Dk_Ability_BasicAttack_Event_Change_End)
	{
		NextComboName = NAME_None;
		Debug::Print(TEXT("Next Combo Name清空了"));
		return;
	}

	TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
	NextComboName = TagNames.Last();
	Debug::Print(FString::Printf(TEXT("Next Combo Name:%s"), *NextComboName.ToString()));
}

void UGA_BasicAttack::DoDamage(FGameplayEventData Data)
{
	int HitResultCount = UAbilitySystemBlueprintLibrary::GetDataCountFromTargetData(Data.TargetData);
	for (int i = 0; i < HitResultCount; ++i)
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(Data.TargetData, i);
		TSubclassOf<UGameplayEffect> EffectForCurrentCombo = GetCurrentEffectForCurrentCombo();
		ApplyGameplayEffectToHitResultActor(
			HitResult, EffectForCurrentCombo, GetAbilityLevel(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo())
		);
	}
}

TSubclassOf<UGameplayEffect> UGA_BasicAttack::GetCurrentEffectForCurrentCombo() const
{
	if (UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance())
	{
		const FName CurrentSectionName = OwnerAnimInstance->Montage_GetCurrentSection(BasicAttackMontage);
		if (const TSubclassOf<UGameplayEffect>* FoundEffectPtr = DamageEffectMap.Find(CurrentSectionName))
		{
			return *FoundEffectPtr;
		}
	}
	return DefaultGameplayEffect;
}
