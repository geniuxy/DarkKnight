// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkGameplayAbilityBase.h"
#include "GA_BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UGA_BasicAttack : public UDkGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_BasicAttack();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	/**********************************************************************/
	/*                               Combo                                */
	/**********************************************************************/
private:
	void SetupWaitComboInputPress();
	
	UFUNCTION()
	void HandleInputPress(float TimeWaited);
	
	void TryCommitCombo();

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* BasicAttackMontage;

	FName NextComboName;
	
	UFUNCTION()
	void ComboChangedEventReceived(FGameplayEventData Data);

	/**********************************************************************/
	/*                               Damage                               */
	/**********************************************************************/
private:
	UPROPERTY(EditAnywhere, Category="Gameplay Effect")
	TSubclassOf<UGameplayEffect> DefaultGameplayEffect;

	UPROPERTY(EditAnywhere, Category="Gameplay Effect")
	TMap<FName, TSubclassOf<UGameplayEffect>> DamageEffectMap;

	UFUNCTION()
	void DoDamage(FGameplayEventData Data);

	TSubclassOf<UGameplayEffect> GetCurrentEffectForCurrentCombo() const;
};
