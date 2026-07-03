// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkGameMenuCenterAreaTask.generated.h"

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
	
	void HandleMainQuestTaskClicked();
	void HandleSideQuestTaskClicked();
	void HandleCompletedTaskClicked();
};
