// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DkAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

void UDkAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, AttackDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, MoveAcceleration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, CarryWeight, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, WeightLimit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDkAttributeSet, Gold, COND_None, REPNOTIFY_Always);
}

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
	const FGameplayEffectContextHandle& Context = Data.EffectSpec.GetContext();
	AActor* EffectCauser = Context.GetEffectCauser();

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

	if (UDkAbilitySystemComponent* OwnerASC = Cast<UDkAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
	{
		if (GetHealth() == 0)
		{
			OwnerASC->IsKilledBy(EffectCauser);
		}
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

void UDkAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, Health, OldValue);
}

void UDkAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, MaxHealth, OldValue);
}

void UDkAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, Energy, OldValue);
}

void UDkAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, MaxEnergy, OldValue);
}

void UDkAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, AttackDamage, OldValue);
}

void UDkAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, Armor, OldValue);
}

void UDkAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, MoveSpeed, OldValue);
}

void UDkAttributeSet::OnRep_MoveAcceleration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, MoveAcceleration, OldValue);
}

void UDkAttributeSet::OnRep_CarryWeight(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, CarryWeight, OldValue);
}

void UDkAttributeSet::OnRep_WeightLimit(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, WeightLimit, OldValue);
}

void UDkAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDkAttributeSet, Gold, OldValue);
}
