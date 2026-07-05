// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/Task/TaskTrackingWidget.h"

#include "CommonTextBlock.h"
#include "Characters/DkCharacterHero.h"
#include "Controllers/DkGamePlayerController.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"

class ADkGamePlayerController;

void UTaskTrackingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	OwnerCharacter = Cast<ADkCharacterHero>(GetOwningPlayerPawn());
	if (ADkGamePlayerController* OwnerPlayerController = Cast<ADkGamePlayerController>(GetOwningPlayer()))
	{
		OwnerPlayerState = OwnerPlayerController->GetPlayerState<ADkPlayerStateBase>();
		OwnerPlayerState->OnUpdateTaskTrackingDelegate.AddUObject(this, &ThisClass::ConfigureTaskTrackingInfo);
	}
}

void UTaskTrackingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ClearTaskTrackingInfo();
}

void UTaskTrackingWidget::ConfigureTaskTrackingInfo(const FText& InTaskName, const FText& InTaskContent, bool bForce)
{
	if (InTaskName.IsEmpty())
	{
		ClearTaskTrackingInfo();
		return;
	}
	if (bForce)
	{
		TaskNameText->SetText(InTaskName);
		TaskContentText->SetText(InTaskContent);
	}
	else
	{
		if (TaskNameText->GetText().EqualTo(InTaskName) || TaskNameText->GetText().IsEmpty())
		{
			TaskNameText->SetText(InTaskName);
			TaskContentText->SetText(InTaskContent);
		}
	}
}

void UTaskTrackingWidget::ClearTaskTrackingInfo()
{
	TaskNameText->SetText(FText::GetEmpty());
	TaskContentText->SetText(FText::GetEmpty());
}
