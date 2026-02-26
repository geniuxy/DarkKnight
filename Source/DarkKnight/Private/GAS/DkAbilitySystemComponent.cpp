// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DkAbilitySystemComponent.h"

void UDkAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	InitStats(OwningAttributeSet, AttributeSetInitialTable);
}
