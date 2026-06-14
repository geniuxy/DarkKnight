// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/Stats/OverHeadStatsGauge.h"

#include "GAS/DkAttributeSet.h"
#include "Widgets/GameHUD/Stats/ValueGauge.h"

void UOverHeadStatsGauge::ConfigureWithASC(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		HealthBar->SendAndBoundToGameplayAttribute(
			AbilitySystemComponent,
			UDkAttributeSet::GetHealthAttribute(),
			UDkAttributeSet::GetMaxHealthAttribute()
		);

		EnergyBar->SendAndBoundToGameplayAttribute(
			AbilitySystemComponent,
			UDkAttributeSet::GetEnergyAttribute(),
			UDkAttributeSet::GetMaxEnergyAttribute()
		);
	}
}
