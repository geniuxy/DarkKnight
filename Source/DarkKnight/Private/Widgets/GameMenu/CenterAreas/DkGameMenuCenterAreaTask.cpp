// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/CenterAreas/DkGameMenuCenterAreaTask.h"

#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Controllers/DkGamePlayerController.h"
#include "FunctionLibrarys/DkTaskFunctionLibrary.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"
#include "Widgets/Components/Buttons/Task/DkUITaskTitleButton.h"
#include "Widgets/Components/Buttons/Task/DkUITaskReferenceButton.h"

void UDkGameMenuCenterAreaTask::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MainQuestTaskTitle->OnClicked().AddUObject(this, &ThisClass::HandleMainQuestTaskClicked);
	SideQuestTaskTitle->OnClicked().AddUObject(this, &ThisClass::HandleSideQuestTaskClicked);
	CompletedTaskTitle->OnClicked().AddUObject(this, &ThisClass::HandleCompletedTaskClicked);

	OwnerPlayerController = Cast<ADkGamePlayerController>(GetOwningPlayer());
}

void UDkGameMenuCenterAreaTask::NativePreConstruct()
{
	Super::NativePreConstruct();

	MainQuestList->SetVisibility(
		MainQuestTaskTitle->GetIsOpen() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed
	);
	SideQuestList->SetVisibility(
		SideQuestTaskTitle->GetIsOpen() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed
	);
	CompletedTaskList->SetVisibility(
		CompletedTaskTitle->GetIsOpen() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed
	);
}

void UDkGameMenuCenterAreaTask::NativeConstruct()
{
	Super::NativeConstruct();

	InsertOrUpdateTaskReference(1);
}

void UDkGameMenuCenterAreaTask::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDkGameMenuCenterAreaTask::HandleMainQuestTaskClicked()
{
	MainQuestTaskTitle->OnTaskTitleClicked();
	bool bIsOpen = MainQuestTaskTitle->GetIsOpen();
	MainQuestList->SetVisibility(bIsOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UDkGameMenuCenterAreaTask::HandleSideQuestTaskClicked()
{
	SideQuestTaskTitle->OnTaskTitleClicked();
	bool bIsOpen = SideQuestTaskTitle->GetIsOpen();
	SideQuestList->SetVisibility(bIsOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UDkGameMenuCenterAreaTask::HandleCompletedTaskClicked()
{
	CompletedTaskTitle->OnTaskTitleClicked();
	bool bIsOpen = CompletedTaskTitle->GetIsOpen();
	CompletedTaskList->SetVisibility(bIsOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UDkGameMenuCenterAreaTask::InsertOrUpdateTaskReference(int InTaskId)
{
	if (TaskReferenceButtonMap.Contains(InTaskId))
	{
		UpdateTaskReference(InTaskId);
	}
	else
	{
		InsertTaskReference(InTaskId);
	}
}

void UDkGameMenuCenterAreaTask::UpdateTaskReference(int InTaskId)
{
	if (!TaskReferenceButtonMap.Contains(InTaskId)) return;

	FTaskInfo CurTaskInfo = UDkTaskFunctionLibrary::GetTaskInfoById(InTaskId);
	bool IsTaskFinished =
		OwnerPlayerController && OwnerPlayerController->GetPlayerState<ADkPlayerStateBase>()->IsTaskFinished(InTaskId);
	UDkUITaskReferenceButton* TaskReferenceButton = TaskReferenceButtonMap.FindRef(InTaskId);
	TaskReferenceButton->ConfigureTaskReference(CurTaskInfo, IsTaskFinished);

	if (IsTaskFinished)
	{
		TaskReferenceButton->RemoveFromParent();
		CompletedTaskList->AddChildToVerticalBox(TaskReferenceButton);
	}
}

void UDkGameMenuCenterAreaTask::InsertTaskReference(int InTaskId)
{
	FTaskInfo CurTaskInfo = UDkTaskFunctionLibrary::GetTaskInfoById(InTaskId);
	bool IsTaskFinished =
		OwnerPlayerController && OwnerPlayerController->GetPlayerState<ADkPlayerStateBase>()->
		                                                IsTaskFinished(InTaskId);
	UDkUITaskReferenceButton* TaskReferenceButton =
		CreateWidget<UDkUITaskReferenceButton>(this, TaskReferenceButtonClass);
	TaskReferenceButton->ConfigureTaskReference(CurTaskInfo, IsTaskFinished);

	UVerticalBoxSlot* VerticalBoxSlot = nullptr;
	if (IsTaskFinished)
	{
		VerticalBoxSlot = CompletedTaskList->AddChildToVerticalBox(TaskReferenceButton);
	}
	else if (CurTaskInfo.TaskType == ETaskType::MainQuest)
	{
		VerticalBoxSlot = MainQuestList->AddChildToVerticalBox(TaskReferenceButton);
	}
	else if (CurTaskInfo.TaskType == ETaskType::SideQuest)
	{
		VerticalBoxSlot = SideQuestList->AddChildToVerticalBox(TaskReferenceButton);
	}
	VerticalBoxSlot->SetPadding(FMargin(5.f));

	TaskReferenceButton->OnTaskReferenceClicked.AddUObject(this, &ThisClass::UpdateTaskDetailInfo);

	TaskReferenceButtonMap.Add(InTaskId, TaskReferenceButton);
}

void UDkGameMenuCenterAreaTask::UpdateTaskDetailInfo(int InTaskId)
{
	FTaskInfo CurTaskInfo = UDkTaskFunctionLibrary::GetTaskInfoById(InTaskId);
	if (CurTaskInfo.TaskId == 0 || CurTaskInfo.TaskId != InTaskId) return;

	TaskName->SetText(CurTaskInfo.TaskName);
}
