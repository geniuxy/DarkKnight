// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Common/GA_Sprint.h"

#include "AbilitySystemComponent.h"
#include "DkGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Settings/DkGameUserSettings.h"

UGA_Sprint::UGA_Sprint()
{
	ActivationOwnedTags.AddTag(DkGameplayTags::Dk_Stats_Sprint);
}

void UGA_Sprint::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility() || !IncreaseMoveSpeedEffect)
	{
		K2_EndAbility();
		return;
	}

	if (K2_HasAuthority())
	{
		IncreaseMoveSpeedEffectHandle = BP_ApplyGameplayEffectToOwner(
			IncreaseMoveSpeedEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo)
		);
	}

	bool bToggle;
	if (IsValid(UDkGameUserSettings::Get()))
	{
		bToggle = UDkGameUserSettings::Get()->IsToggleSprintMode();
	}
	else
	{
		bToggle = bIsToggle;
	}
	if (bToggle)
	{
		UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
		WaitInputPress->OnPress.AddDynamic(this, &ThisClass::HandleInputPress);
		WaitInputPress->ReadyForActivation();
	}
	else
	{
		UAbilityTask_WaitInputRelease* WaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
		WaitInputRelease->OnRelease.AddDynamic(this, &ThisClass::HandleInputRelease);
		WaitInputRelease->ReadyForActivation();
	}
}

void UGA_Sprint::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	UAbilitySystemComponent* OwnerAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (OwnerAbilitySystemComponent && IncreaseMoveSpeedEffectHandle.IsValid())
	{
		OwnerAbilitySystemComponent->RemoveActiveGameplayEffect(IncreaseMoveSpeedEffectHandle);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Sprint::HandleInputPress(float TimeWaited)
{
	K2_EndAbility();
}

void UGA_Sprint::HandleInputRelease(float TimeHeld)
{
	K2_EndAbility();
}
