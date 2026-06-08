// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkGameplayAbilityBase.h"
#include "GA_DrawWeapon_Primary.generated.h"

class UAbilityTask_WaitInputPress;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UGA_DrawWeapon_Primary : public UDkGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UGA_DrawWeapon_Primary();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	UFUNCTION()
	void HandleInputPress(float TimeWaited);
	UFUNCTION()
	void OnSheatheSwordEnd();
	
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	UAnimMontage* DrawSwordMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Anim")
	UAnimMontage* SheatheSwordMontage; // 收剑动作

	UPROPERTY()
	UAbilityTask_WaitInputPress* WaitInputPress;
};
