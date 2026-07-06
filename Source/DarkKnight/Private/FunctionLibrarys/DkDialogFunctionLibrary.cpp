// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkDialogFunctionLibrary.h"

#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"

bool UDkDialogFunctionLibrary::HasTaskToCommit(int InDialogId)
{
	TMap<int, FDialogContent> DialogContentInfoMap = UDkDataSubsystem::Get()->GetDialogContentInfo();
	if (!DialogContentInfoMap.Contains(InDialogId)) return false;
	FDialogContent DialogContent = DialogContentInfoMap.FindRef(InDialogId);
	FCommitTaskDetails* CommitTaskDetails = DialogContent.CommitTaskDetails.GetMutablePtr<FCommitTaskDetails>();
	return CommitTaskDetails && CommitTaskDetails->MainTaskId != 0 && CommitTaskDetails->SubTaskId != 0;
}

FCommitTaskDetails UDkDialogFunctionLibrary::GetDialogCommitTaskDetails(int InDialogId)
{
	TMap<int, FDialogContent> DialogContentInfoMap = UDkDataSubsystem::Get()->GetDialogContentInfo();
	if (!DialogContentInfoMap.Contains(InDialogId)) return FCommitTaskDetails();
	FDialogContent DialogContent = DialogContentInfoMap.FindRef(InDialogId);
	FCommitTaskDetails* CommitTaskDetails = DialogContent.CommitTaskDetails.GetMutablePtr<FCommitTaskDetails>();
	return *CommitTaskDetails;
}
