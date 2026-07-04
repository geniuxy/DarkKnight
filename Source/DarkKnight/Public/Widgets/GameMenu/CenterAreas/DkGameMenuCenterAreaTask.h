// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkStructs.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkGameMenuCenterAreaTask.generated.h"

class UDkWidgetSubTaskTarget;
class ADkGamePlayerController;
class UDkUITaskReferenceButton;
class UCommonTextBlock;
class UCommonLazyImage;
class UVerticalBox;
class UDkUITaskTitleButton;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkGameMenuCenterAreaTask : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End UUserWidget Function

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UDkUITaskTitleButton* MainQuestTaskTitle;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UVerticalBox* MainQuestList;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UDkUITaskTitleButton* SideQuestTaskTitle;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UVerticalBox* SideQuestList;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UDkUITaskTitleButton* CompletedTaskTitle;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UVerticalBox* CompletedTaskList;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonLazyImage* TaskIcon;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TaskName;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TaskArea;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UVerticalBox* SubTaskList;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TaskDescription;
	//***** Bound Widgets *****//

	UPROPERTY()
	TObjectPtr<ADkGamePlayerController> OwnerPlayerController;

	UPROPERTY()
	TMap<int, UDkUITaskReferenceButton*> TaskReferenceButtonMap; 
	
	void HandleMainQuestTaskClicked();
	void HandleSideQuestTaskClicked();
	void HandleCompletedTaskClicked();

	void InsertOrUpdateTaskReference(int InTaskId);
	void UpdateTaskReference(int InTaskId);
	void InsertTaskReference(int InTaskId);
	void ClearTaskDetailInfo();
	void UpdateTaskDetailInfo(int InTaskId);
	void UpdateSubTaskList(int InMainTaskId, TArray<FSubTaskInfo> InSubTaskList);

	UPROPERTY(EditDefaultsOnly, Category="Task Reference")
	TSubclassOf<UDkUITaskReferenceButton> TaskReferenceButtonClass;

	UPROPERTY(EditDefaultsOnly, Category="Sub Task")
	TSubclassOf<UDkWidgetSubTaskTarget> SubTaskTargetClass;
};
