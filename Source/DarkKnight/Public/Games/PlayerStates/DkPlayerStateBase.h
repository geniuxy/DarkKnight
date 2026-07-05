// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DkActionComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkStructs.h"
#include "GameFramework/PlayerState.h"
#include "DkPlayerStateBase.generated.h"

class UDkTaskComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnAddTaskNotice, ETaskNoticeState /*InTaskNoticeState*/, const FText& /*InTaskName*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddOrUpdateTask, int /*InTaskId*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(
	FOnUpdateTaskTracking, const FText& /*InTaskNoticeState*/, const FText& /*InTaskName*/, bool /*bForce*/)
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API ADkPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	ADkPlayerStateBase();

	FOnAddTaskNotice OnAddTaskNoticeDelegate;
	FOnAddOrUpdateTask OnAddOrUpdateTaskDelegate;
	FOnUpdateTaskTracking OnUpdateTaskTrackingDelegate;

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
	TArray<int> GetAllPlayerTaskId() const;
	FText GetSubTaskDescription(int InMainTaskId, int InSubTaskId) const;
	int GetCurSubTaskId(int InMainTaskId) const;

protected:
	UPROPERTY()
	UDkTaskComponent* PlayerTaskComponent;
};
