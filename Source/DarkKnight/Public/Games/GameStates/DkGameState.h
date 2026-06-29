// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DkGameState.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API ADkGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
