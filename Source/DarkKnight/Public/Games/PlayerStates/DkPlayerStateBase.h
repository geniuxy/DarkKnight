// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DkActionComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkStructs.h"
#include "GameFramework/PlayerState.h"
#include "DkPlayerStateBase.generated.h"

class UDkTaskComponent;
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

	/**********************************************************************/
	/*                                Task                                */
	/**********************************************************************/
public:
	bool IsTaskFinished(int InTaskId) const;
	ETaskState GetSubTaskState(int InMainTaskId, int InSubTaskId) const;
	int GetSubTaskProgress(int InMainTaskId, int InSubTaskId) const;

protected:
	UPROPERTY()
	UDkTaskComponent* PlayerTaskComponent;
};
