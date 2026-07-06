// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkTaskFunctionLibrary.h"

#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"

FTaskInfo UDkTaskFunctionLibrary::GetTaskInfoById(int InTaskId)
{
	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (TaskInfoMap.Contains(InTaskId))
	{
		return TaskInfoMap.FindRef(InTaskId);
	}
	return FTaskInfo();
}

TArray<int> UDkTaskFunctionLibrary::GetAllSubTaskId(int InMainTaskId)
{
	TArray<int> SubTaskIdList;

	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InMainTaskId)) return SubTaskIdList;

	FTaskInfo CurTaskInfo = TaskInfoMap.FindRef(InMainTaskId);
	for (FSubTaskInfo SubTaskInfo : CurTaskInfo.SubTaskList)
	{
		SubTaskIdList.Add(SubTaskInfo.SubTaskId);
	}
	return SubTaskIdList;
}

int UDkTaskFunctionLibrary::GetSubTaskTarget(int InMainTaskId, int InSubTaskId)
{
	int Target = 0;
	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InMainTaskId)) return 0;
	FTaskInfo TaskInfo = TaskInfoMap.FindRef(InMainTaskId);
	for (const FSubTaskInfo& SubTaskInfo : TaskInfo.SubTaskList)
	{
		if (SubTaskInfo.SubTaskId == InSubTaskId)
		{
			Target = SubTaskInfo.TargetProgress;
			break;
		}
	}
	return Target;
}

bool UDkTaskFunctionLibrary::IsNextSubTaskIdZero(int InMainTaskId, int InSubTaskId)
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

int UDkTaskFunctionLibrary::GetNextSubTaskId(int InMainTaskId, int InSubTaskId)
{
	TMap<int, FTaskInfo> TaskInfoMap = UDkDataSubsystem::Get()->GetTaskInfo();
	if (!TaskInfoMap.Contains(InMainTaskId)) return false;

	for (const FSubTaskInfo& SubTaskInfo : TaskInfoMap[InMainTaskId].SubTaskList)
	{
		if (SubTaskInfo.SubTaskId == InSubTaskId)
		{
			return SubTaskInfo.NextSubTaskId;
		}
	}

	return 0;
}
