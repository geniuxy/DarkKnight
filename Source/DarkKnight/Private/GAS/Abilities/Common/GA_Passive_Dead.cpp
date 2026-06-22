// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Common/GA_Passive_Dead.h"

#include "DkGameplayTags.h"

UGA_Passive_Dead::UGA_Passive_Dead()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = DkGameplayTags::Dk_Ability_Passive_Dead_Event_Activate;

	AbilityTriggers.Add(TriggerData);

	ActivationBlockedTags.RemoveTag(DkGameplayTags::Dk_Stats_Stun);
}

void UGA_Passive_Dead::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// TODO: 可以的用来给周围单位增加经验，参考Crunch
}
