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
