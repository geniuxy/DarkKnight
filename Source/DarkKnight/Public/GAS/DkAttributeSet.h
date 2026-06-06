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
 * 
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
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Attack);
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
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Damage);
	
	// 用于GameplayEffect的Modifiers相关计算Clamp
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 用于GameplayEffect的Executions相关计算Clamp
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void RescaleHealth();
	void RescaleEnergy();
	
protected:
	UPROPERTY()
	FGameplayAttributeData CachedHealthPercent;
	UPROPERTY()
	FGameplayAttributeData CachedEnergyPercent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Level;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData HealthRecovery;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Attack;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Armor;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData SpellPower;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData PhysicsDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MagicDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData AttackSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MoveSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MoveAcceleration;
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
	FGameplayAttributeData Energy;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxEnergy;
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
	FGameplayAttributeData CarryWeight;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData WeightLimit;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ExecutionChance;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MonsterExperienceAcquisition;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ShopSellingPrice;
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Damage;
};
