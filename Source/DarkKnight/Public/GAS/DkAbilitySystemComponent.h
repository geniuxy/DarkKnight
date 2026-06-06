// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DkAbilitySystemComponent.generated.h"

class UPA_AbilitySystemGenerics;
enum class EAbilityInputID : uint8;
/**
 * 
 */
UCLASS(Blueprintable)
class DARKKNIGHT_API UDkAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UDkAbilitySystemComponent();

	void ServerSideInit();

protected:
	virtual void BeginPlay() override;

private:
	void InitializeBaseAttributes();
	void InitializeBaseGameplayEffects();
	void GiveInitialAbilities();
	
	void AuthApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int Level = 1);
	
	void HealthUpdated(const FOnAttributeChangeData& ChangeData);
	void EnergyUpdated(const FOnAttributeChangeData& ChangeData);

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Ability")
	TMap<EAbilityInputID, TSubclassOf<UGameplayAbility>> Abilities; // 可学习的Abilities（初始0级）

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Ability")
	TMap<EAbilityInputID, TSubclassOf<UGameplayAbility>> BasicAbilities; // 初始Abilities（初始1级）

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Ability")
	UPA_AbilitySystemGenerics* AbilitySystemGenerics;

public:
	FORCEINLINE const TMap<EAbilityInputID, TSubclassOf<UGameplayAbility>>& GetAbilities() const { return Abilities; }
};
