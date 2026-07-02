// Fill out your copyright notice in the Description page of Project Settings.


#include "Games/PlayerStates/DkPlayerStateBase.h"

#include "Components/DkTaskComponent.h"

ADkPlayerStateBase::ADkPlayerStateBase()
{
	SetNetUpdateFrequency(100.f);
	
	PlayerTaskComponent = CreateDefaultSubobject<UDkTaskComponent>("PlayerTaskComponent");
}

void ADkPlayerStateBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerTaskComponent->UpdatePlayerTaskCompletionStatus();
}
