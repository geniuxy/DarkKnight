// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/DkGameplayAbilityBase.h"
#include "GA_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UGA_Sprint : public UDkGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_Sprint();
	
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
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UGameplayEffect> IncreaseMoveSpeedEffect;

	FActiveGameplayEffectHandle IncreaseMoveSpeedEffectHandle;

	UPROPERTY(EditDefaultsOnly, Category="Ability Details")
	bool bIsToggle;

	UFUNCTION()
	void HandleInputPress(float TimeWaited);

	UFUNCTION()
	void HandleInputRelease(float TimeHeld);
};
