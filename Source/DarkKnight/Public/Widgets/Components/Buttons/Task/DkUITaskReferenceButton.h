// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "DkTypes/DkStructs.h"
#include "DkUITaskReferenceButton.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;
class UBorder;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTaskReferenceClicked, int /* InTaskId */)
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUITaskReferenceButton : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	void ConfigureTaskReference(const FTaskInfo& InTaskInfo, bool IsTaskFinished);

	FOnTaskReferenceClicked OnTaskReferenceClicked;

protected:
	//~Begin UCommonButtonBase UFunction
	virtual void NativeOnClicked() override;
	//~End UCommonButtonBase UFunction

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UBorder* TaskFlagColor;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonLazyImage* TaskIcon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TaskNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TaskAreaText;
	//***** Bound Widgets *****//

	UPROPERTY()
	int TaskId;

	void UpdateTaskNameText(const FTaskInfo& InTaskInfo, bool bIsTaskFinished);
	void UpdateTaskFlagColor(ETaskType InTaskType, bool bIsTaskFinished);

	UPROPERTY(EditDefaultsOnly, Category="Task Name Color")
	TSubclassOf<UCommonTextStyle> CompletedTaskNameStyle;

	UPROPERTY(EditDefaultsOnly, Category="Task Name Color")
	TSubclassOf<UCommonTextStyle> InProgressTaskNameStyle;

	UPROPERTY(EditDefaultsOnly, Category="Task Flag")
	FLinearColor MainQuestFlagColor;

	UPROPERTY(EditDefaultsOnly, Category="Task Flag")
	FLinearColor SideQuestFlagColor;

	UPROPERTY(EditDefaultsOnly, Category="Task Flag")
	FLinearColor CompletedFlagColor;
};
