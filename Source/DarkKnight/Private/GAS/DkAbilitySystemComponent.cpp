// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DkAbilitySystemComponent.h"

#include "GAS/DkAttributeSet.h"

void UDkAbilitySystemComponent::AbilityActorInfoSet()
{
	// 用于ApplyGameplayEffectToSelf后，在客户端还会执行一些操作
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDkAbilitySystemComponent::ClientEffectApplied);
}

void UDkAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDkAbilitySystemComponent::ClientEffectApplied_Implementation(
	UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& GameplayEffectSpec,
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AssetTagContainer;
	GameplayEffectSpec.GetAllAssetTags(AssetTagContainer);

	// EffectAssetsTagDelegate.Broadcast(AssetTagContainer);
}
