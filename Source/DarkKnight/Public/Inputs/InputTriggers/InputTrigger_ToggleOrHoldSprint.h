// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "InputTrigger_ToggleOrHoldSprint.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UInputTrigger_ToggleOrHoldSprint : public UInputTriggerTimedBase
{
	GENERATED_BODY()

public:
	virtual ETriggerState UpdateState_Implementation(
		const UEnhancedPlayerInput* PlayerInput,
		FInputActionValue ModifiedValue,
		float DeltaTime
	) override;

private:
	UPROPERTY(EditDefaultsOnly)
	bool bIsToggle = true;
};
