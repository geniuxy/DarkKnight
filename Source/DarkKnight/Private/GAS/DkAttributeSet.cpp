// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DkAttributeSet.h"
#include "GameplayEffectExtension.h"

void UDkAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxEnergy());
	}
}

void UDkAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
		SetCachedHealthPercent(GetHealth() / GetMaxHealth());
	}
	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0, GetMaxEnergy()));
		SetCachedEnergyPercent(GetEnergy() / GetMaxEnergy());
	}
}

void UDkAttributeSet::RescaleHealth()
{
	if (!GetOwningActor() || !GetOwningActor()->HasAuthority()) return;

	if (GetCachedHealthPercent() != 0.f && GetHealth() != 0.f)
	{
		SetHealth(FMath::Clamp(GetMaxHealth() * GetCachedHealthPercent(), 0, GetMaxHealth()));
	}
}

void UDkAttributeSet::RescaleEnergy()
{
	if (!GetOwningActor() || !GetOwningActor()->HasAuthority()) return;

	if (GetCachedEnergyPercent() != 0.f && GetEnergy() != 0.f)
	{
		SetEnergy(FMath::Clamp(GetMaxEnergy() * GetCachedEnergyPercent(), 0, GetMaxEnergy()));
	}
}
