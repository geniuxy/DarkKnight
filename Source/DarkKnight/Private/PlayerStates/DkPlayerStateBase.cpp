// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/DkPlayerStateBase.h"

#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkAttributeSet.h"
#include "Net/UnrealNetwork.h"

ADkPlayerStateBase::ADkPlayerStateBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDkAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UDkAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void ADkPlayerStateBase::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitStats(UDkAttributeSet::StaticClass(), AttributeSetInitialTable);
}
