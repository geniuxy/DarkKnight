// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_DrawWeapon_Primary.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "DkGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

UGA_DrawWeapon_Primary::UGA_DrawWeapon_Primary()
{
	// NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	//
	// FAbilityTriggerData TriggerData;
	// TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	// TriggerData.TriggerTag = DkGameplayTags::Dk_Ability_Passive_HoldSword_Event_Activate;
	// AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(DkGameplayTags::Dk_Stats_EquippingSword);
	ActivationRequiredTags.AddTag(DkGameplayTags::Dk_Stats_Equipped_PrimaryWeapon);
}

void UGA_DrawWeapon_Primary::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility() || !GetOwnerASC())
	{
		K2_EndAbility();
		return;
	}

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayDrawSwordMontage =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DrawSwordMontage);
		// PlayDrawSwordMontage->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		// PlayDrawSwordMontage->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		// PlayDrawSwordMontage->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		// PlayDrawSwordMontage->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayDrawSwordMontage->ReadyForActivation();
	}

	WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &ThisClass::HandleInputPress);
	WaitInputPress->ReadyForActivation();
}

void UGA_DrawWeapon_Primary::HandleInputPress(float TimeWaited)
{
	if (!OwnerASC->HasMatchingGameplayTag(DkGameplayTags::Dk_Stats_InCombat))
	{
		WaitInputPress->EndTask();
		WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
		WaitInputPress->OnPress.AddDynamic(this, &ThisClass::HandleInputPress);
		WaitInputPress->ReadyForActivation();
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlaySheatheSwordMontage =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SheatheSwordMontage);
	PlaySheatheSwordMontage->OnCompleted.AddDynamic(this, &ThisClass::OnSheatheSwordEnd);
	PlaySheatheSwordMontage->OnBlendOut.AddDynamic(this, &ThisClass::OnSheatheSwordEnd);
	PlaySheatheSwordMontage->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
	PlaySheatheSwordMontage->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
	PlaySheatheSwordMontage->ReadyForActivation();
}

void UGA_DrawWeapon_Primary::OnSheatheSwordEnd()
{
	OwnerASC->RemoveActiveEffectsWithGrantedTags(
		FGameplayTagContainer(DkGameplayTags::Dk_Stats_InCombat)
	);

	OwnerASC->RemoveActiveEffectsWithGrantedTags(
		FGameplayTagContainer(DkGameplayTags::Dk_Stats_CanAttack)
	);

	K2_EndAbility();
}
