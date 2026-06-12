// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/DkGameplayAbilityBase.h"
#include "GA_LockTarget.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UGA_LockTarget : public UDkGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_LockTarget();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	float LockTargetCheckDistance = 3000.f;

	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	float LockTargetCheckRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	float LockTargetCheckTimeInterval = 0.1f;

	FTimerHandle LockTargetCheckTimerHandle;
	
	bool bWasSprinting = false; // 瞄准前正在疾跑

	void FindLockTarget();
	void StartLockTargetCheckTimer();
	void StopLockTargetCheckTimer();
	bool IsTargetInRange() const;
	void TargetDeadTagUpdated(const FGameplayTag Tag, int32 NewCount);

	UFUNCTION()
	void HandleInputPress(float TimeWaited);
};
