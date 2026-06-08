// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Passive_HoldSword.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "DkGameplayTags.h"

UGA_Passive_HoldSword::UGA_Passive_HoldSword()
{
	// NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	//
	// FAbilityTriggerData TriggerData;
	// TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	// TriggerData.TriggerTag = DkGameplayTags::Dk_Ability_Passive_HoldSword_Event_Activate;
	// AbilityTriggers.Add(TriggerData);

	ActivationOwnedTags.AddTag(DkGameplayTags::Dk_Stats_DrawingSword);
}

void UGA_Passive_HoldSword::ActivateAbility(
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
		UAbilityTask_PlayMontageAndWait* PlayGroundBlastMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DrawSwordMontage);
		PlayGroundBlastMontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayGroundBlastMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayGroundBlastMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayGroundBlastMontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayGroundBlastMontageTask->ReadyForActivation();
	}
}
