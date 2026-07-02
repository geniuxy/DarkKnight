// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DkTypes/DkStructs.h"
#include "DkTaskComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkTaskComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkTaskComponent();

	void UpdatePlayerTaskCompletionStatus();

protected:
	virtual void BeginPlay() override;

private:
	TMap<int, FTaskCompletionStatus> CurrentTaskCompletionStatus;
};
