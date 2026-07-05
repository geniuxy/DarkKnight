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

bool ADkPlayerStateBase::IsTaskFinished(int InTaskId) const
{
	return PlayerTaskComponent && PlayerTaskComponent->IsTaskFinished(InTaskId);
}

ETaskState ADkPlayerStateBase::GetSubTaskState(int InMainTaskId, int InSubTaskId) const
{
	if (PlayerTaskComponent)
	{
		return PlayerTaskComponent->GetSubTaskState(InMainTaskId, InSubTaskId);
	}
	return ETaskState::None;
}

int ADkPlayerStateBase::GetSubTaskProgress(int InMainTaskId, int InSubTaskId) const
{
	if (PlayerTaskComponent)
	{
		return PlayerTaskComponent->GetSubTaskProgress(InMainTaskId, InSubTaskId);
	}
	return 0;
}

TArray<int> ADkPlayerStateBase::GetAllPlayerTaskId() const
{
	TArray<int> Results;
	if (PlayerTaskComponent)
	{
		Results = PlayerTaskComponent->GetAllPlayerTaskId();
	}
	return Results;
}
