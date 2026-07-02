// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkTaskComponent.h"

#include "DarkKnightDebugHelper.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"


UDkTaskComponent::UDkTaskComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CachedTaskTags.Reset();
}

void UDkTaskComponent::UpdatePlayerTaskCompletionStatus()
{
	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	for (TTuple<int, FTaskInfo> TaskInfoPair : TaskInfoMap)
	{
		if (!CurrentTaskCompletionStatus.Contains(TaskInfoPair.Key))
		{
			FTaskInfo TaskInfo = TaskInfoPair.Value;
			FTaskCompletionStatus Status;
			Status.TaskId = TaskInfo.TaskId;
			Status.TaskState = ETaskState::None;
			if (HasFinishedAllPreconditionTask(TaskInfo.PreconditionTags))
			{
				Status.TaskState = ETaskState::ToBeAccepted;
			}
			// TODO: 如果未来加入存档功能，可以在这更改初始任务状态
			for (FSubTaskInfo SubTaskInfo : TaskInfo.SubTaskList)
			{
				FSubTaskCompletionStatus SubTaskCompletionStatus;
				SubTaskCompletionStatus.SubTaskId = SubTaskInfo.SubTaskId;
				SubTaskCompletionStatus.SubTaskState = ETaskState::None;
				for (TTuple<int, int> ProgressPair : SubTaskInfo.TargetProgress)
				{
					SubTaskCompletionStatus.CurrentProgress.Add(ProgressPair.Key, 0);
				}
				Status.SubTaskCompletionList.Add(SubTaskCompletionStatus);
			}
			CurrentTaskCompletionStatus.Add(TaskInfoPair.Key, Status);
		}
	}
}

void UDkTaskComponent::AcceptTask(int InTaskId)
{
	if (!CurrentTaskCompletionStatus.Contains(InTaskId)) return;

	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InTaskId)) return;

	FTaskInfo CurTaskInfo = TaskInfoMap.FindRef(InTaskId);
	if (!CurTaskInfo.PreconditionTags.IsEmpty() && !HasFinishedAllPreconditionTask(CurTaskInfo.PreconditionTags))
	{
		return;
	}

	UpdateTaskState(InTaskId, ETaskState::InProgress);
	UpdateSubTaskState(InTaskId, 1, ETaskState::InProgress);
}

void UDkTaskComponent::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TempTimeHandle;
	GetWorld()->GetTimerManager().SetTimer(TempTimeHandle, FTimerDelegate::CreateLambda([this]()
	{
		AcceptTask(1);
	}), 0.2f, false);
}

bool UDkTaskComponent::HasFinishedAllPreconditionTask(const FGameplayTagContainer& InTagContainer) const
{
	return CachedTaskTags.HasAllExact(InTagContainer);
}

void UDkTaskComponent::UpdateTaskState(int InTaskId, ETaskState InTaskState)
{
	if (!CurrentTaskCompletionStatus.Contains(InTaskId)) return;

	CurrentTaskCompletionStatus[InTaskId].TaskState = InTaskState;
}

void UDkTaskComponent::UpdateSubTaskState(int InMainTaskId, int InSubTaskId, ETaskState InTaskState)
{
	if (!CurrentTaskCompletionStatus.Contains(InMainTaskId)) return;
	FTaskCompletionStatus* MainTaskCompletionStatus = CurrentTaskCompletionStatus.Find(InMainTaskId);

	FSubTaskCompletionStatus* SubTaskStatus = MainTaskCompletionStatus->SubTaskCompletionList.FindByPredicate(
		[InSubTaskId](const FSubTaskCompletionStatus& SubTaskCompletion)-> bool
		{
			return SubTaskCompletion.SubTaskId == InSubTaskId;
		}
	);
	SubTaskStatus->SubTaskState = InTaskState;

	if (InTaskState == ETaskState::Completed && IsNextSubTaskIdZero(InMainTaskId, InSubTaskId))
	{
		UpdateTaskState(InMainTaskId, ETaskState::Completed);
	}
}

bool UDkTaskComponent::IsNextSubTaskIdZero(int InMainTaskId, int InSubTaskId) const
{
	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InMainTaskId)) return false;

	for (const FSubTaskInfo& SubTaskInfo : TaskInfoMap[InMainTaskId].SubTaskList)
	{
		if (SubTaskInfo.SubTaskId == InSubTaskId)
		{
			return SubTaskInfo.NextSubTaskId == 0;
		}
	}

	return true;
}
