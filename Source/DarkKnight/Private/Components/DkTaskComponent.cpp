// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkTaskComponent.h"

#include "Characters/DkCharacterHero.h"
#include "Components/DkInventoryComponent.h"
#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"
#include "FunctionLibrarys/DkTaskFunctionLibrary.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"
#include "Inventory/DkInventoryItem.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"


UDkTaskComponent::UDkTaskComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CachedTaskTags.Reset();
}

void UDkTaskComponent::InitPlayerTaskCompletionStatus()
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
			for (int i = 0; i < TaskInfo.SubTaskList.Num(); ++i)
			{
				FSubTaskInfo SubTaskInfo = TaskInfo.SubTaskList[i];
				FSubTaskCompletionStatus SubTaskCompletionStatus;
				SubTaskCompletionStatus.SubTaskId = SubTaskInfo.SubTaskId;
				SubTaskCompletionStatus.SubTaskState = i == 0 ? ETaskState::ToBeAccepted : ETaskState::None;
				SubTaskCompletionStatus.CurrentProgress = 0;
				Status.SubTaskCompletionList.Add(SubTaskCompletionStatus);
			}
			CurrentTaskCompletionStatus.Add(TaskInfoPair.Key, Status);
		}
	}
}

void UDkTaskComponent::CommitTask(int InTaskId, int InSubTaskId, int CommitCount)
{
	if (!CurrentTaskCompletionStatus.Contains(InTaskId)) return;

	ETaskState TaskState = GetTaskState(InTaskId);
	ETaskState SubTaskState = GetSubTaskState(InTaskId, InSubTaskId);
	if (TaskState == ETaskState::Completed || SubTaskState == ETaskState::Completed || SubTaskState == ETaskState::None)
	{
		return;
	}

	if (TaskState == ETaskState::ToBeAccepted && SubTaskState == ETaskState::ToBeAccepted)
	{
		AcceptTask(InTaskId);
	}
	else if (TaskState == ETaskState::InProgress && SubTaskState == ETaskState::InProgress)
	{
		int SubTaskTarget = UDkTaskFunctionLibrary::GetSubTaskTarget(InTaskId, InSubTaskId);
		int CurSubTaskProgress = GetSubTaskProgress(InTaskId, InSubTaskId) + CommitCount;
		if (CurSubTaskProgress >= SubTaskTarget)
		{
			if (UDkTaskFunctionLibrary::IsNextSubTaskIdZero(InTaskId, InSubTaskId))
			{
				CompleteTask(InTaskId);
			}
			else
			{
				CompleteSubTask(InTaskId, InSubTaskId);
			}
		}
		else
		{
			UpdateSubTaskProgress(InTaskId, InSubTaskId, CurSubTaskProgress);
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

void UDkTaskComponent::UpdateSubTaskProgress(int InTaskId, int InSubTaskId, int CurProgress)
{
	if (!CurrentTaskCompletionStatus.Contains(InTaskId)) return;

	FTaskCompletionStatus* TaskCompletionStatus = CurrentTaskCompletionStatus.Find(InTaskId);
	for (FSubTaskCompletionStatus& SubTaskCompletionStatus : TaskCompletionStatus->SubTaskCompletionList)
	{
		if (SubTaskCompletionStatus.SubTaskId == InSubTaskId)
		{
			SubTaskCompletionStatus.CurrentProgress = CurProgress;
		}
	}

	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InTaskId)) return;
	FTaskInfo CurTaskInfo = TaskInfoMap.FindRef(InTaskId);

	if (OwnerPlayerState)
	{
		OwnerPlayerState->OnAddOrUpdateTaskDelegate.Broadcast(InTaskId);
		OwnerPlayerState->OnUpdateTaskTrackingDelegate.Broadcast(
			CurTaskInfo.TaskName, GetSubTaskDescription(InTaskId, InSubTaskId), false
		);
	}
}

void UDkTaskComponent::CompleteSubTask(int InTaskId, int InSubTaskId)
{
	if (!CurrentTaskCompletionStatus.Contains(InTaskId)) return;

	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InTaskId)) return;

	FTaskInfo CurTaskInfo = TaskInfoMap.FindRef(InTaskId);
	if (!CurTaskInfo.PreconditionTags.IsEmpty() && !HasFinishedAllPreconditionTask(CurTaskInfo.PreconditionTags))
	{
		return;
	}

	UpdateSubTaskState(InTaskId, InSubTaskId, ETaskState::Completed);
	int NextSubTaskId = UDkTaskFunctionLibrary::GetNextSubTaskId(InTaskId, InSubTaskId);
	UpdateSubTaskState(InTaskId, NextSubTaskId, ETaskState::InProgress);

	if (OwnerPlayerState)
	{
		OwnerPlayerState->OnAddTaskNoticeDelegate.Broadcast(ETaskNoticeState::TaskUpdate, CurTaskInfo.TaskName);
		OwnerPlayerState->OnAddOrUpdateTaskDelegate.Broadcast(InTaskId);
		OwnerPlayerState->OnUpdateTaskTrackingDelegate.Broadcast(
			CurTaskInfo.TaskName, GetSubTaskDescription(InTaskId, NextSubTaskId), false
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
		if (GetSubTaskState(InTaskId, SubTaskId) != ETaskState::Completed)
		{
			UpdateSubTaskState(InTaskId, SubTaskId, ETaskState::Completed);
		}
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

ETaskState UDkTaskComponent::GetTaskState(int InMainTaskId) const
{
	if (!CurrentTaskCompletionStatus.Contains(InMainTaskId)) return ETaskState::None;

	return CurrentTaskCompletionStatus[InMainTaskId].TaskState;
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

void UDkTaskComponent::TryGetTaskRewards(int InMainTaskId)
{
	TMap<int, int> TaskRewardsInfo = UDkTaskFunctionLibrary::GetTaskRewardsInfo(InMainTaskId);

	for (TTuple<int, int> RewardInfo : TaskRewardsInfo)
	{
		AddItemToOwnerInventory(RewardInfo.Key, RewardInfo.Value);
	}
}

void UDkTaskComponent::TryGetSubTaskRewards(int InMainTaskId, int InSubTaskId)
{
	TMap<int, int> SubTaskRewardsInfo = UDkTaskFunctionLibrary::GetSubTaskRewardsInfo(InMainTaskId, InSubTaskId);

	for (TTuple<int, int> RewardInfo : SubTaskRewardsInfo)
	{
		AddItemToOwnerInventory(RewardInfo.Key, RewardInfo.Value);
	}
}

void UDkTaskComponent::AddItemToOwnerInventory(int InItemId, int InItemStack)
{
	OwnerInventoryComp = UDkInventorySubsystem::Get()->GetCachedInventoryComponent();
	if (IsValid(OwnerInventoryComp))
	{
		if (!UDkInventoryFunctionLibrary::IsItemStackable(InItemId))
		{
			for (int i = 0; i < InItemStack; ++i)
			{
				TryAddItem(InItemId, 1);
			}
		}
		else
		{
			TryAddItem(InItemId, InItemStack);
		}
	}
}

void UDkTaskComponent::TryAddItem(int InItemId, int InItemStack)
{
	if (!IsValid(OwnerInventoryComp)) return;

	UDkInventoryItem* RewardItem = UDkInventoryFunctionLibrary::SpawnInventoryItemById(
		OwnerInventoryComp, InItemId, InItemStack
	);
	if (IsValid(RewardItem))
	{
		OwnerInventoryComp->TryAddItem(RewardItem);
	}
}

void UDkTaskComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayerState = Cast<ADkPlayerStateBase>(GetOwner());
	if (OwnerPlayerState)
	{
		OwnerPlayerState->OnCommitTaskDelegate.AddUObject(this, &ThisClass::CommitTask);
	}
}

ADkCharacterHero* UDkTaskComponent::GetOwnerCharacter()
{
	if (OwnerPlayerState)
	{
		OwnerCharacter = Cast<ADkCharacterHero>(OwnerPlayerState->GetPawn());
		return OwnerCharacter;
	}
	return nullptr;
}

bool UDkTaskComponent::HasFinishedAllPreconditionTask(const FGameplayTagContainer& InTagContainer) const
{
	return CachedTaskTags.HasAllExact(InTagContainer);
}

void UDkTaskComponent::UpdateTaskState(int InTaskId, ETaskState InTaskState)
{
	if (!CurrentTaskCompletionStatus.Contains(InTaskId)) return;

	CurrentTaskCompletionStatus[InTaskId].TaskState = InTaskState;

	if (InTaskState == ETaskState::Completed)
	{
		TryGetTaskRewards(InTaskId);
	}
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

	if (InTaskState == ETaskState::Completed)
	{
		SubTaskStatus->CurrentProgress = UDkTaskFunctionLibrary::GetSubTaskTarget(InMainTaskId, InSubTaskId);
		TryGetSubTaskRewards(InMainTaskId, InSubTaskId);
	}
}
