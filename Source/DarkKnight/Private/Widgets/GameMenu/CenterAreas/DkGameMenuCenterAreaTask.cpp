// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/CenterAreas/DkGameMenuCenterAreaTask.h"

#include "Components/VerticalBox.h"
#include "Widgets/Components/Buttons/Task/DkUITaskTitleButton.h"

void UDkGameMenuCenterAreaTask::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MainQuestTaskTitle->OnClicked().AddUObject(this, &ThisClass::HandleMainQuestTaskClicked);
	SideQuestTaskTitle->OnClicked().AddUObject(this, &ThisClass::HandleSideQuestTaskClicked);
	CompletedTaskTitle->OnClicked().AddUObject(this, &ThisClass::HandleCompletedTaskClicked);
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
