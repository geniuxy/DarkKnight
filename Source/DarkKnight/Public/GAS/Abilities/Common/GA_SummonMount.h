// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/DkGameplayAbilityBase.h"
#include "GA_SummonMount.generated.h"

class ADkMountBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UGA_SummonMount : public UDkGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_SummonMount();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	void SpawnNewMount();
	void UpdateInstigator(ADkMountBase* CachedMount);
	FVector FindPlaceToSummonMount();
	bool TryFindSummonLocationInDirection(const FVector& Direction, FVector& OutLocation);
	bool CheckSummonLocationAtPoint(const FVector& CheckPoint, FVector& OutLocation);

	UPROPERTY(EditDefaultsOnly, Category="Spawn Mount")
	float CheckDistance = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category="Spawn Mount")
	TSubclassOf<ADkMountBase> MountClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn Mount")
	USoundBase* SummonSoundEffect;
};
