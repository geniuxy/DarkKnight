// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkTaskComponent.h"

#include "DarkKnightDebugHelper.h"
#include "Characters/DkCharacterHero.h"
#include "FunctionLibrarys/DkTaskFunctionLibrary.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"
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
				SubTaskCompletionStatus.SubTaskState = ETaskState::ToBeAccepted;
				SubTaskCompletionStatus.CurrentProgress = 0;
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

	if (OwnerPlayerState)
	{
		OwnerPlayerState->OnAddTaskNoticeDelegate.Broadcast(ETaskNoticeState::NewTask, CurTaskInfo.TaskName);
		OwnerPlayerState->OnAddOrUpdateTaskDelegate.Broadcast(InTaskId);
		OwnerPlayerState->OnUpdateTaskTrackingDelegate.Broadcast(
			CurTaskInfo.TaskName, GetSubTaskDescription(InTaskId, 1), false
		);
	}
}

void UDkTaskComponent::UpdateTask(int InTaskId, int InSubTaskId)
{
	if (!CurrentTaskCompletionStatus.Contains(InTaskId)) return;

	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InTaskId)) return;

	FTaskInfo CurTaskInfo = TaskInfoMap.FindRef(InTaskId);
	if (!CurTaskInfo.PreconditionTags.IsEmpty() && !HasFinishedAllPreconditionTask(CurTaskInfo.PreconditionTags))
	{
		return;
	}

	UpdateSubTaskState(InTaskId, InSubTaskId - 1, ETaskState::Completed);
	UpdateSubTaskState(InTaskId, InSubTaskId, ETaskState::InProgress);

	if (OwnerPlayerState)
	{
		OwnerPlayerState->OnAddTaskNoticeDelegate.Broadcast(ETaskNoticeState::TaskUpdate, CurTaskInfo.TaskName);
		OwnerPlayerState->OnAddOrUpdateTaskDelegate.Broadcast(InTaskId);
		OwnerPlayerState->OnUpdateTaskTrackingDelegate.Broadcast(
			CurTaskInfo.TaskName, GetSubTaskDescription(InTaskId, InSubTaskId), false
		);
	}
}

void UDkTaskComponent::CompleteTask(int InTaskId)
{
	if (!CurrentTaskCompletionStatus.Contains(InTaskId)) return;

	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InTaskId)) return;
	FTaskInfo CurTaskInfo = TaskInfoMap.FindRef(InTaskId);
	if (!CurTaskInfo.PreconditionTags.IsEmpty() && !HasFinishedAllPreconditionTask(CurTaskInfo.PreconditionTags))
	{
		return;
	}

	UpdateTaskState(InTaskId, ETaskState::Completed);
	for (int SubTaskId : UDkTaskFunctionLibrary::GetAllSubTaskId(InTaskId))
	{
		UpdateSubTaskState(InTaskId, SubTaskId, ETaskState::Completed);
	}

	if (OwnerPlayerState)
	{
		OwnerPlayerState->OnAddTaskNoticeDelegate.Broadcast(ETaskNoticeState::TaskCompleted, CurTaskInfo.TaskName);
		OwnerPlayerState->OnAddOrUpdateTaskDelegate.Broadcast(InTaskId);
		OwnerPlayerState->OnUpdateTaskTrackingDelegate.Broadcast(
			FText::GetEmpty(), FText::GetEmpty(), false
		);
	}
}

bool UDkTaskComponent::IsTaskFinished(int InTaskId) const
{
	if (!CurrentTaskCompletionStatus.Contains(InTaskId)) return false;

	return CurrentTaskCompletionStatus[InTaskId].TaskState == ETaskState::Completed;
}

ETaskState UDkTaskComponent::GetSubTaskState(int InMainTaskId, int InSubTaskId) const
{
	if (!CurrentTaskCompletionStatus.Contains(InMainTaskId)) return ETaskState::None;

	ETaskState SubTaskState = ETaskState::None;
	TArray<FSubTaskCompletionStatus> SubTaskCompletionList =
		CurrentTaskCompletionStatus[InMainTaskId].SubTaskCompletionList;
	for (FSubTaskCompletionStatus SubTaskCompletionStatus : SubTaskCompletionList)
	{
		if (SubTaskCompletionStatus.SubTaskId == InSubTaskId)
		{
			SubTaskState = SubTaskCompletionStatus.SubTaskState;
			break;
		}
	}
	return SubTaskState;
}

int UDkTaskComponent::GetSubTaskProgress(int InMainTaskId, int InSubTaskId) const
{
	if (!CurrentTaskCompletionStatus.Contains(InMainTaskId)) return 0;

	int CurProgress = 0;
	TArray<FSubTaskCompletionStatus> SubTaskCompletionList =
		CurrentTaskCompletionStatus[InMainTaskId].SubTaskCompletionList;
	for (FSubTaskCompletionStatus SubTaskCompletionStatus : SubTaskCompletionList)
	{
		if (SubTaskCompletionStatus.SubTaskId == InSubTaskId)
		{
			CurProgress = SubTaskCompletionStatus.CurrentProgress;
			break;
		}
	}
	return CurProgress;
}

TArray<int> UDkTaskComponent::GetAllPlayerTaskId() const
{
	TArray<int> Results;
	for (TTuple<int, FTaskCompletionStatus> CompletionPair : CurrentTaskCompletionStatus)
	{
		if (CompletionPair.Value.TaskState != ETaskState::ToBeAccepted)
		{
			Results.Add(CompletionPair.Key);
		}
	}
	return Results;
}

FText UDkTaskComponent::GetSubTaskDescription(int InMainTaskId, int InSubTaskId) const
{
	if (!CurrentTaskCompletionStatus.Contains(InMainTaskId)) return FText::GetEmpty();
	const FTaskCompletionStatus* MainTaskCompletionStatus = CurrentTaskCompletionStatus.Find(InMainTaskId);

	const FSubTaskCompletionStatus* SubTaskStatus = MainTaskCompletionStatus->SubTaskCompletionList.FindByPredicate(
		[InSubTaskId](const FSubTaskCompletionStatus& SubTaskCompletion)-> bool
		{
			return SubTaskCompletion.SubTaskId == InSubTaskId;
		}
	);
	if (!SubTaskStatus || SubTaskStatus->SubTaskId == 0) return FText::GetEmpty();

	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InMainTaskId)) return FText::GetEmpty();

	FSubTaskInfo CurSubTaskInfo;
	for (const FSubTaskInfo& SubTaskInfo : TaskInfoMap[InMainTaskId].SubTaskList)
	{
		if (SubTaskInfo.SubTaskId == InSubTaskId)
		{
			CurSubTaskInfo = SubTaskInfo;
			break;
		}
	}
	if (CurSubTaskInfo.SubTaskId == 0) return FText::GetEmpty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("A"), SubTaskStatus->CurrentProgress);
	Args.Add(TEXT("B"), CurSubTaskInfo.TargetProgress);
	return FText::Format(CurSubTaskInfo.SubTaskDescription, Args);
}

int UDkTaskComponent::GetCurSubTaskId(int InMainTaskId) const
{
	if (!CurrentTaskCompletionStatus.Contains(InMainTaskId)) return 0;

	int OutSubTaskId = 0;
	TArray<FSubTaskCompletionStatus> SubTaskCompletionList =
		CurrentTaskCompletionStatus[InMainTaskId].SubTaskCompletionList;
	for (FSubTaskCompletionStatus SubTaskCompletionStatus : SubTaskCompletionList)
	{
		if (SubTaskCompletionStatus.SubTaskState == ETaskState::InProgress)
		{
			OutSubTaskId = SubTaskCompletionStatus.SubTaskId;
			break;
		}
	}
	return OutSubTaskId;
}

void UDkTaskComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayerState = Cast<ADkPlayerStateBase>(GetOwner());

	FTimerHandle TempTimeHandle;
	GetWorld()->GetTimerManager().SetTimer(TempTimeHandle, FTimerDelegate::CreateLambda([=, this]()
	{
		AcceptTask(1);
		AcceptTask(2);
	}), 5.f, false);

	FTimerHandle TempTimeHandle1;
	GetWorld()->GetTimerManager().SetTimer(TempTimeHandle1, FTimerDelegate::CreateLambda([this]()
	{
		UpdateTask(1, 2);
	}), 10.f, false);

	// FTimerHandle TempTimeHandle2;
	// GetWorld()->GetTimerManager().SetTimer(TempTimeHandle2, FTimerDelegate::CreateLambda([this]()
	// {
	// 	CompleteTask(1);
	// }), 15.f, false);
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
	if (SubTaskStatus->SubTaskId == 0) return;

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
