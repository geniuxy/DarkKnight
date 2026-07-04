// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/CenterAreas/DkGameMenuCenterAreaTask.h"

#include "Components/VerticalBox.h"
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

	InsertTaskReference(1);
}

void UDkGameMenuCenterAreaTask::NativeDestruct()
{
	Super::NativeDestruct();

	MainQuestList->ClearChildren();
	SideQuestList->ClearChildren();
	CompletedTaskList->ClearChildren();
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

void UDkGameMenuCenterAreaTask::InsertTaskReference(int InTaskId)
{
	FTaskInfo CurTaskInfo = UDkTaskFunctionLibrary::GetTaskInfoById(InTaskId);
	bool IsTaskFinished =
		OwnerPlayerController && OwnerPlayerController->GetPlayerState<ADkPlayerStateBase>()->IsTaskFinished(InTaskId);

	UDkUITaskReferenceButton* TaskReferenceButton =
		CreateWidget<UDkUITaskReferenceButton>(this, TaskReferenceButtonClass);
	TaskReferenceButton->ConfigureTaskReference(CurTaskInfo, IsTaskFinished);

	if (IsTaskFinished)
	{
		CompletedTaskList->AddChildToVerticalBox(TaskReferenceButton);
	}
	else if (CurTaskInfo.TaskType == ETaskType::MainQuest)
	{
		MainQuestList->AddChildToVerticalBox(TaskReferenceButton);
	}
	else if (CurTaskInfo.TaskType == ETaskType::SideQuest)
	{
		SideQuestList->AddChildToVerticalBox(TaskReferenceButton);
	}
}
