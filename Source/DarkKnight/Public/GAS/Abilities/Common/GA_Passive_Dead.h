// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/DkGameplayAbilityBase.h"
#include "GA_Passive_Dead.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UGA_Passive_Dead : public UDkGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_Passive_Dead();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
};
