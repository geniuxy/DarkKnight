// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/Task/DkUITaskReferenceButton.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Components/Border.h"
#include "FunctionLibrarys/DkCommonFunctionLibrary.h"

void UDkUITaskReferenceButton::ConfigureTaskReference(const FTaskInfo& InTaskInfo, bool IsTaskFinished)
{
	TaskId = InTaskInfo.TaskId;
	
	UpdateTaskNameText(InTaskInfo, IsTaskFinished);
	TaskAreaText->SetText(UDkCommonFunctionLibrary::GetDisplayValueOfEnum(InTaskInfo.TaskAreaType));
	TaskIcon->SetBrushFromTexture(InTaskInfo.TaskIcon);
	UpdateTaskFlagColor(InTaskInfo.TaskType, IsTaskFinished);
}

void UDkUITaskReferenceButton::NativeOnClicked()
{
	Super::NativeOnClicked();

	OnTaskReferenceClicked.Broadcast(TaskId);
}

void UDkUITaskReferenceButton::UpdateTaskNameText(const FTaskInfo& InTaskInfo, bool bIsTaskFinished)
{
	TaskNameText->SetText(InTaskInfo.TaskName);
	if (bIsTaskFinished)
	{
		TaskNameText->SetStyle(CompletedTaskNameStyle);
	}
	else
	{
		TaskNameText->SetStyle(InProgressTaskNameStyle);
	}
}

void UDkUITaskReferenceButton::UpdateTaskFlagColor(ETaskType InTaskType, bool bIsTaskFinished)
{
	if (bIsTaskFinished)
	{
		TaskFlagColor->SetBrushColor(CompletedFlagColor);
	}
	else if (InTaskType == ETaskType::MainQuest)
	{
		TaskFlagColor->SetBrushColor(MainQuestFlagColor);
	}
	else if (InTaskType == ETaskType::SideQuest)
	{
		TaskFlagColor->SetBrushColor(SideQuestFlagColor);
	}
	else
	{
		TaskFlagColor->SetBrushColor(CompletedFlagColor);
	}
}
