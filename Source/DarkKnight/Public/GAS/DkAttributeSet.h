// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "DkAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * Tips:
 * 1. 控制台输入 AbilitySystem.DebugAttribute Health MaxHealth (可以实时debug)
 */
UCLASS()
class DARKKNIGHT_API UDkAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Level);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, CachedHealthPercent)
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, HealthRecovery);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, AttackDamage);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Armor);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, SpellPower);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, PhysicsDamage);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MagicDamage);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, AttackSpeed);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MoveSpeed);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MoveAcceleration);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, CritRate);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, CritDamage);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, PunctureResistance);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, ChopResistance);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, BluntImpactResistance);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Toxin);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxToxin);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Energy);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxEnergy);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, CachedEnergyPercent)
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, EnergyRecovery);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, EnergyRecoveryBonus);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Adrenaline);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxAdrenaline);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, AdrenalineRecoveryBonus);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, BreakingDefenseValue);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxBreakingDefenseValue);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, CarryWeight);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, WeightLimit);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, ExecutionChance);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MonsterExperienceAcquisition);
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, ShopSellingPrice);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 用于GameplayEffect的Modifiers相关计算Clamp
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 用于GameplayEffect的Executions相关计算Clamp
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void RescaleHealth();
	void RescaleEnergy();
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_Energy)
	FGameplayAttributeData Energy;
	UPROPERTY(ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	UPROPERTY(ReplicatedUsing = OnRep_AttackDamage)
	FGameplayAttributeData AttackDamage;
	UPROPERTY(ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	UPROPERTY(ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	UPROPERTY(ReplicatedUsing = OnRep_MoveAcceleration)
	FGameplayAttributeData MoveAcceleration;
	UPROPERTY(ReplicatedUsing = OnRep_CarryWeight)
	FGameplayAttributeData CarryWeight;
	UPROPERTY(ReplicatedUsing = OnRep_WeightLimit)
	FGameplayAttributeData WeightLimit;
	
	UPROPERTY()
	FGameplayAttributeData CachedHealthPercent;
	UPROPERTY()
	FGameplayAttributeData CachedEnergyPercent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Level;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData HealthRecovery;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData SpellPower;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData PhysicsDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MagicDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData AttackSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData CritRate;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData CritDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData PunctureResistance;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ChopResistance;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData BluntImpactResistance;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Toxin;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxToxin;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData EnergyRecovery;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData EnergyRecoveryBonus;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Adrenaline;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxAdrenaline;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData AdrenalineRecoveryBonus;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData BreakingDefenseValue;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxBreakingDefenseValue;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ExecutionChance;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MonsterExperienceAcquisition;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ShopSellingPrice;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_AttackDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MoveAcceleration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_CarryWeight(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeightLimit(const FGameplayAttributeData& OldValue);
};
