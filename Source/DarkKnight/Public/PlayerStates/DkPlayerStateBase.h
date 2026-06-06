// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DkActionComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "GameFramework/PlayerState.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkAttributeSet.h"
#include "DkPlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API ADkPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	ADkPlayerStateBase();

protected:
	virtual void BeginPlay() override;

	EActionState CurrentActionState; // 记录目前玩家的动作状态

public:
	LIST_DATA_ACCESSOR(EActionState, CurrentActionState)
};
