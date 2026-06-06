// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "DkHeroAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkHeroAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, Intelligence)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, Strength)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, Experience)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, PrevLevelExperience)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, NextLevelExperience)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, Level)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, UpgradePoint)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, MaxLevel)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, MaxLevelExperience)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, Gold)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, IntelligenceGrowthRate)
	ATTRIBUTE_ACCESSORS(UDkHeroAttributeSet, StrengthGrowthRate)

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData Intelligence;

	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData Strength;

	UPROPERTY()
	FGameplayAttributeData IntelligenceGrowthRate;

	UPROPERTY()
	FGameplayAttributeData StrengthGrowthRate;

	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData Experience;

	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData PrevLevelExperience;

	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData NextLevelExperience;

	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData Level;

	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData UpgradePoint;

	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData MaxLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData MaxLevelExperience;
	
	UPROPERTY(BlueprintReadOnly, Category = "Hero Attribute")
	FGameplayAttributeData Gold;
};
