// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/DkGameplayAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Characters/DkCharacterBase.h"
#include "FunctionLibrarys/DkAbilitySystemFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"
#include "GAS/DkAbilitySystemComponent.h"

UDkGameplayAbilityBase::UDkGameplayAbilityBase()
{
	ActivationBlockedTags.AddTag(DkGameplayTags::Dk_Stats_Stun);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDkGameplayAbilityBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TaskToCommit.MainTaskId != 0 && TaskToCommit.SubTaskId != 0)
	{
		if (GetOwnerAvatarCharacter() && GetOwnerAvatarCharacter()->IsLocallyControlled())
		{
			if (ADkPlayerStateBase* OwnerPlayerState = GetOwnerAvatarCharacter()->GetPlayerState<ADkPlayerStateBase>())
			{
				OwnerPlayerState->OnCommitTaskDelegate.Broadcast(
					TaskToCommit.MainTaskId,
					TaskToCommit.SubTaskId,
					TaskToCommit.CommitCount
				);
			}
		}
	}
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

UAbilitySystemComponent* UDkGameplayAbilityBase::GetOwnerASC()
{
	if (!OwnerASC)
	{
		OwnerASC = Cast<UAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	}

	return OwnerASC;
}

void UDkGameplayAbilityBase::SendLocalGameplayEvent(const FGameplayTag& EventTag, const FGameplayEventData& EventData)
{
	if (GetOwnerASC())
	{
		OwnerASC->HandleGameplayEvent(EventTag, &EventData);
	}
}

void UDkGameplayAbilityBase::EndActivatingAbility(FGameplayTag InAbilityTag)
{
	FGameplayTagContainer CancelTags;
	CancelTags.AddTag(InAbilityTag);
	if (GetOwnerASC())
	{
		OwnerASC->CancelAbilities(&CancelTags, nullptr, nullptr);
	}
}

void UDkGameplayAbilityBase::TryToActivateAbility(FGameplayTag InAbilityTag)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(InAbilityTag);
	if (GetOwnerASC())
	{
		OwnerASC->TryActivateAbilitiesByTag(TagContainer);
	}
}

void UDkGameplayAbilityBase::ApplyGameplayEffectToHitResultActor(
	const FHitResult& HitResult, const TSubclassOf<UGameplayEffect>& InGameplayEffect, int Level)
{
	if (!IsValid(InGameplayEffect)) return;
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(InGameplayEffect, Level);

	FGameplayEffectContextHandle EffectContext =
		MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
	EffectContext.AddHitResult(HitResult);

	EffectSpecHandle.Data->SetContext(EffectContext);

	ApplyGameplayEffectSpecToTarget(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		EffectSpecHandle,
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult)
	);
}

AActor* UDkGameplayAbilityBase::GetClosetTarget(float AimDistance, ETeamAttitude::Type TeamAttitude) const
{
	if (AActor* OwnerAvatarActor = GetAvatarActorFromActorInfo())
	{
		FVector Location;
		FRotator Rotation;
		OwnerAvatarActor->GetActorEyesViewPoint(Location, Rotation);

		FVector AimEnd = Location + Rotation.Vector() * AimDistance;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(OwnerAvatarActor);

		FCollisionObjectQueryParams CollisionObjectQueryParams;
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		if (ShouldDrawDebug())
		{
			DrawDebugLine(GetWorld(), Location, AimEnd, FColor::Red, false, 2.f, 0, 3.f);
		}

		TArray<FHitResult> HitResults;
		if (GetWorld()->LineTraceMultiByObjectType(
			HitResults, Location, AimEnd, CollisionObjectQueryParams, CollisionQueryParams))
		{
			for (const FHitResult& HitResult : HitResults)
			{
				if (IsOtherActorTeamAttitudeIs(HitResult.GetActor(), TeamAttitude))
				{
					return HitResult.GetActor();
				}
			}
		}
	}
	return nullptr;
}

AActor* UDkGameplayAbilityBase::GetClosetTargetInView(
	float InDistance, float InRadius, ETeamAttitude::Type TeamAttitude) const
{
	AActor* OwnerAvatarActor = GetAvatarActorFromActorInfo();
	if (!OwnerAvatarActor)
	{
		return nullptr;
	}

	FVector Location;
	FRotator Rotation;
	OwnerAvatarActor->GetActorEyesViewPoint(Location, Rotation);

	const FVector ForwardVector = Rotation.Vector();
	const FVector TargetingEnd = Location + ForwardVector * InDistance;

	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(GetClosetTargetInView), false);
	CollisionQueryParams.AddIgnoredActor(OwnerAvatarActor);

	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	if (ShouldDrawDebug())
	{
		DrawDebugCylinder(GetWorld(), Location, TargetingEnd, InRadius, 12, FColor::Red, false, 2.f, 0, 3.f);
	}

	TArray<FHitResult> HitResults;
	const bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		Location,
		TargetingEnd,
		FQuat::Identity, // 圆柱体不需要旋转（沿Z轴方向），这里用Identity，因为方向由起点和终点决定
		CollisionObjectQueryParams,
		FCollisionShape::MakeCapsule(InRadius, InDistance * 0.5f), // 用胶囊体模拟圆柱体
		CollisionQueryParams
	);

	AActor* ClosestTarget = nullptr;
	float ClosestDistanceSq = MAX_flt;

	if (bHit)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (!HitActor || HitActor == OwnerAvatarActor)
			{
				continue;
			}

			if (!IsOtherActorTeamAttitudeIs(HitActor, TeamAttitude))
			{
				continue;
			}

			if (UDkAbilitySystemFunctionLibrary::IsActorDead(Cast<ADkCharacterBase>(HitActor)))
			{
				continue;
			}

			const FVector ToTarget = HitActor->GetActorLocation() - OwnerAvatarActor->GetActorLocation();
			const float DistToStartSq = ToTarget.SizeSquared();
			if (DistToStartSq < ClosestDistanceSq)
			{
				ClosestDistanceSq = DistToStartSq;
				ClosestTarget = HitActor;
			}
		}
	}

	return ClosestTarget;
}

AActor* UDkGameplayAbilityBase::GetCurrentLockTarget() const
{
	UDkAbilitySystemComponent* OwnerDkASC = Cast<UDkAbilitySystemComponent>(
		IsValid(OwnerASC) ? OwnerASC : GetAbilitySystemComponentFromActorInfo()
	);
	if (OwnerDkASC)
	{
		return OwnerDkASC->GetLockTarget();
	}

	return nullptr;
}

void UDkGameplayAbilityBase::SetCurrentLockTarget(AActor* NewLockTarget)
{
	UDkAbilitySystemComponent* OwnerDkASC = Cast<UDkAbilitySystemComponent>(
		IsValid(OwnerASC) ? OwnerASC : GetAbilitySystemComponentFromActorInfo()
	);
	if (OwnerDkASC)
	{
		OwnerDkASC->SetLockTarget(NewLockTarget);
	}
}

UAbilitySystemComponent* UDkGameplayAbilityBase::GetCurrentLockTargetASC()
{
	if (AActor* CurrentLockTarget = GetCurrentLockTarget())
	{
		return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CurrentLockTarget);
	}
	return nullptr;
}

bool UDkGameplayAbilityBase::HasValidLockTarget() const
{
	if (!GetCurrentLockTarget()) return false;
	if (UDkAbilitySystemFunctionLibrary::IsActorDead(GetCurrentLockTarget())) return false;

	return true;
}
