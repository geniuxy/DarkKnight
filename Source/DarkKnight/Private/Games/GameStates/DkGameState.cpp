// Fill out your copyright notice in the Description page of Project Settings.


#include "Games/GameStates/DkGameState.h"

#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"

void ADkGameState::BeginPlay()
{
	Super::BeginPlay();

	UDkDataSubsystem::Get()->InitializeData();
	UDkInventorySubsystem::Get()->InitializeData();
}
