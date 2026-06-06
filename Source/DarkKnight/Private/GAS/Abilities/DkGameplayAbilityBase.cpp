// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/DkGameplayAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "DkGameplayTags.h"
#include "GameFramework/Character.h"

UDkGameplayAbilityBase::UDkGameplayAbilityBase()
{
	ActivationBlockedTags.AddTag(DkGameplayTags::Dk_Stats_Stun);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UDkGameplayAbilityBase::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	// 当技能为0级(未学习)时，不能使用、触发
	FGameplayAbilitySpec* AbilitySpec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
	if (AbilitySpec && AbilitySpec->Level <= 0)
	{
		return false;
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

UAnimInstance* UDkGameplayAbilityBase::GetOwnerAnimInstance() const
{
	USkeletalMeshComponent* OwnerSkeletalMeshComp = GetOwningComponentFromActorInfo();
	if (OwnerSkeletalMeshComp)
	{
		return OwnerSkeletalMeshComp->GetAnimInstance();
	}
	return nullptr;
}

FGenericTeamId UDkGameplayAbilityBase::GetOwnerTeamID() const
{
	IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());
	if (OwnerTeamInterface)
	{
		return OwnerTeamInterface->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

bool UDkGameplayAbilityBase::IsOtherActorTeamAttitudeIs(
	const AActor* OtherActor, ETeamAttitude::Type TargetTeamAttitude) const
{
	if (!OtherActor) return false;

	IGenericTeamAgentInterface* OwnerTeamAgentInterface =
		Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());
	if (OwnerTeamAgentInterface)
	{
		return OwnerTeamAgentInterface->GetTeamAttitudeTowards(*OtherActor) == TargetTeamAttitude;
	}

	return false;
}

ACharacter* UDkGameplayAbilityBase::GetOwnerAvatarCharacter()
{
	if (!OwnerAvatarCharacter)
	{
		OwnerAvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	}

	return OwnerAvatarCharacter;
}

void UDkGameplayAbilityBase::SendLocalGameplayEvent(const FGameplayTag& EventTag, const FGameplayEventData& EventData)
{
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	if (OwnerASC)
	{
		OwnerASC->HandleGameplayEvent(EventTag, &EventData);
	}
}
