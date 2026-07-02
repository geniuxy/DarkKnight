// Fill out your copyright notice in the Description page of Project Settings.


#include "Games/GameStates/DkGameState.h"

#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"

void ADkGameState::BeginPlay()
{
	Super::BeginPlay();

	UDkDataSubsystem::Get()->InitializeDialogContent();
	UDkDataSubsystem::Get()->InitializeNpcInfo();
	UDkDataSubsystem::Get()->InitializeTaskInfo();
}
