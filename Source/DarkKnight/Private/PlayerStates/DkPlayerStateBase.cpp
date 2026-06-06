// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/DkPlayerStateBase.h"

#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkAttributeSet.h"
#include "Net/UnrealNetwork.h"

ADkPlayerStateBase::ADkPlayerStateBase()
{
	SetNetUpdateFrequency(100.f);
}

void ADkPlayerStateBase::BeginPlay()
{
	Super::BeginPlay();
}
