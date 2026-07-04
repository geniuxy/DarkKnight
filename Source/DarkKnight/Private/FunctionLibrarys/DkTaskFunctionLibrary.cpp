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
