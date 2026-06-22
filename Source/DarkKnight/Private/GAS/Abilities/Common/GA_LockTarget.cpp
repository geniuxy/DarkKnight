// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Common/GA_LockTarget.h"

#include "AbilitySystemComponent.h"
#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "FunctionLibrarys/DkAbilitySystemFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Settings/DkGameUserSettings.h"

UGA_LockTarget::UGA_LockTarget()
{
	AbilityTags.AddTag(DkGameplayTags::Dk_Ability_LockTarget_Tag);
	ActivationOwnedTags.AddTag(DkGameplayTags::Dk_Stats_LockingTarget);
	ActivationOwnedTags.AddTag(DkGameplayTags::Dk_Stats_Crosshair_LockTarget);
	// TODO: 加个DisableLockTarget的tag
	ActivationBlockedTags.AddTag(DkGameplayTags::Dk_Stats_LockingTarget);
}

void UGA_LockTarget::ActivateAbility(
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

	Debug::Print(TEXT("开始锁定敌人"));

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		FindLockTarget();
	}

	if (HasValidLockTarget())
	{
		if (UDkAbilitySystemFunctionLibrary::ActorHasTag(GetOwnerAvatarCharacter(), DkGameplayTags::Dk_Stats_Sprint))
		{
			bWasSprinting = true;
			EndActivatingAbility(DkGameplayTags::Dk_Ability_Sprint_Tag);
		}
		StartLockTargetCheckTimer();
		UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
		WaitInputPress->OnPress.AddDynamic(this, &ThisClass::HandleInputPress);
		WaitInputPress->ReadyForActivation();
	}
	else
	{
		K2_EndAbility();
	}
}

void UGA_LockTarget::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Debug::Print(TEXT("结束锁定敌人"));
	if (UAbilitySystemComponent* PrevLockTargetASC = GetCurrentLockTargetASC())
	{
		PrevLockTargetASC->RegisterGameplayTagEvent(DkGameplayTags::Dk_Stats_Dead).RemoveAll(this);
	}
	SetCurrentLockTarget(nullptr);

	if (bWasSprinting && UDkGameUserSettings::Get()->IsToggleSprintMode())
	{
		TryToActivateAbility(DkGameplayTags::Dk_Ability_Sprint_Tag);
	}

	StopLockTargetCheckTimer();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_LockTarget::FindLockTarget()
{
	if (HasValidLockTarget()) return;
	if (IsTargetInRange()) return;

	if (UAbilitySystemComponent* PrevLockTargetASC = GetCurrentLockTargetASC())
	{
		PrevLockTargetASC->RegisterGameplayTagEvent(DkGameplayTags::Dk_Stats_Dead).RemoveAll(this);
	}

	SetCurrentLockTarget(GetClosetTargetInView(LockTargetCheckDistance, LockTargetCheckRadius, ETeamAttitude::Hostile));
	if (UAbilitySystemComponent* CurrentLockTargetASC = GetCurrentLockTargetASC())
	{
		CurrentLockTargetASC->RegisterGameplayTagEvent(DkGameplayTags::Dk_Stats_Dead).AddUObject(
			this, &ThisClass::TargetDeadTagUpdated
		);
	}
}

void UGA_LockTarget::StartLockTargetCheckTimer()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(
			LockTargetCheckTimerHandle, this, &ThisClass::FindLockTarget, LockTargetCheckTimeInterval, true
		);
	}
}

void UGA_LockTarget::StopLockTargetCheckTimer()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(LockTargetCheckTimerHandle);
	}
}

bool UGA_LockTarget::IsTargetInRange() const
{
	if (!HasValidLockTarget()) return false;

	float Distance = FVector::Distance(
		GetCurrentLockTarget()->GetActorLocation(), GetAvatarActorFromActorInfo()->GetActorLocation()
	);
	return Distance <= LockTargetCheckDistance;
}

void UGA_LockTarget::TargetDeadTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FindLockTarget();
		if (!HasValidLockTarget())
		{
			K2_EndAbility();
		}
	}
}

void UGA_LockTarget::HandleInputPress(float TimeWaited)
{
	Debug::Print(TEXT("再次按下中间，解除锁定目标"));
	K2_EndAbility();
}
