// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkGameplayAbilityBase.h"
#include "GA_Passive_HoldSword.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UGA_Passive_HoldSword : public UDkGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_Passive_HoldSword();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	UAnimMontage* DrawSwordMontage;
};
