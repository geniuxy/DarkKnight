// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GenericTeamAgentInterface.h"
#include "DkTypes/DkStructs.h"
#include "DkGameplayAbilityBase.generated.h"

class ADkPlayerStateBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDkGameplayAbilityBase();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr
	) const override;

protected:
	UAnimInstance* GetOwnerAnimInstance() const;

	UPROPERTY(EditDefaultsOnly, Category="debug")
	bool bShouldDrawDebug = false;

	FGenericTeamId GetOwnerTeamID() const;

	bool IsOtherActorTeamAttitudeIs(const AActor* OtherActor, ETeamAttitude::Type TargetTeamAttitude) const;

	ACharacter* GetOwnerAvatarCharacter();

	UPROPERTY()
	ACharacter* OwnerAvatarCharacter;

	UAbilitySystemComponent* GetOwnerASC();

	UPROPERTY()
	UAbilitySystemComponent* OwnerASC;

	ADkPlayerStateBase* GetOwnerPlayerState();
	
	UPROPERTY()
	ADkPlayerStateBase* OwnerPlayerState;

	void SendLocalGameplayEvent(const FGameplayTag& EventTag, const FGameplayEventData& EventData);

	void EndActivatingAbility(FGameplayTag InAbilityTag);
	void TryToActivateAbility(FGameplayTag InAbilityTag);

	void ApplyGameplayEffectToHitResultActor(
		const FHitResult& HitResult, const TSubclassOf<UGameplayEffect>& InGameplayEffect, int Level = 1
	);

public:
	FORCEINLINE bool ShouldDrawDebug() const { return bShouldDrawDebug; }

	/**********************************************************************/
	/*                             Targeting                              */
	/**********************************************************************/
protected:
	AActor* GetClosetTarget(float AimDistance, ETeamAttitude::Type TeamAttitude = ETeamAttitude::Hostile) const;

	AActor* GetClosetTargetInView(
		float InDistance, float InRadius, ETeamAttitude::Type TeamAttitude = ETeamAttitude::Hostile
	) const;

	AActor* GetCurrentLockTarget() const;
	void SetCurrentLockTarget(AActor* NewLockTarget);
	UAbilitySystemComponent* GetCurrentLockTargetASC();
	bool HasValidLockTarget() const;

	/**********************************************************************/
	/*                           Commit Task                              */
	/**********************************************************************/
protected:
	UPROPERTY(EditAnywhere, Category="Commit Task")
	FCommitTaskDetails TaskToCommit;
};
