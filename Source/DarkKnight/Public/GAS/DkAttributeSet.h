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
	UDkAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Level);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData HealthRecovery;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, HealthRecovery);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Attack);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData SpellPower;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, SpellPower);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData PhysicsDamage;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, PhysicsDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MagicDamage;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MagicDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData CritRate;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, CritRate);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, CritDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData PunctureResistance;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, PunctureResistance);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ChopResistance;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, ChopResistance);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData BluntImpactResistance;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, BluntImpactResistance);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Toxin;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Toxin);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxToxin;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxToxin);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Energy);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxEnergy);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData EnergyRecovery;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, EnergyRecovery);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData EnergyRecoveryBonus;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, EnergyRecoveryBonus);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Adrenaline;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Adrenaline);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxAdrenaline;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxAdrenaline);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData AdrenalineRecoveryBonus;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, AdrenalineRecoveryBonus);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData BreakingDefenseValue;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, BreakingDefenseValue);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MaxBreakingDefenseValue;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MaxBreakingDefenseValue);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData CarryWeight;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, CarryWeight);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData WeightLimit;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, WeightLimit);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ExecutionChance;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, ExecutionChance);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData MonsterExperienceAcquisition;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, MonsterExperienceAcquisition);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData ShopSellingPrice;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, ShopSellingPrice);
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UDkAttributeSet, Damage);

};
