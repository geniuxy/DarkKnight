// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/Task/TaskNoticeWidget.h"

#include "CommonTextBlock.h"
#include "Characters/DkCharacterHero.h"
#include "Controllers/DkGamePlayerController.h"
#include "FunctionLibrarys/DkAbilitySystemFunctionLibrary.h"
#include "FunctionLibrarys/DkCommonFunctionLibrary.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsytems/DkUISubsystem.h"

void UTaskNoticeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OwnerCharacter = Cast<ADkCharacterHero>(GetOwningPlayerPawn());
	if (ADkGamePlayerController* OwnerPlayerController = Cast<ADkGamePlayerController>(GetOwningPlayer()))
	{
		OwnerPlayerState = OwnerPlayerController->GetPlayerState<ADkPlayerStateBase>();
		if (OwnerPlayerState)
		{
			OwnerPlayerState->OnAddTaskNoticeDelegate.AddUObject(this, &ThisClass::AddTaskNotice);
		}
	}
}

void UTaskNoticeWidget::OnAnimationStarted_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation == ShowNoticeAnimation)
	{
		bIsPlayingNotice = true;
	}
}

void UTaskNoticeWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation == ShowNoticeAnimation)
	{
		bIsPlayingNotice = false;
	}
}

void UTaskNoticeWidget::AddTaskNotice(ETaskNoticeState InTaskNoticeState, const FText& InTaskName)
{
	FTaskNoticeInfo TaskNoticeInfo;
	TaskNoticeInfo.TaskNoticeState = InTaskNoticeState;
	TaskNoticeInfo.TaskName = InTaskName;

	CachedTaskNoticeList.Add(TaskNoticeInfo);

	GetWorld()->GetTimerManager().SetTimer(TaskNoticeTimerHandle, this, &ThisClass::TryPlayTaskNotice, 0.5f, true, 0.f);
}

void UTaskNoticeWidget::TryPlayTaskNotice()
{
	if (bIsPlayingNotice) return;
	if (UDkUISubsystem::Get(this)->IsGameMenuOpen()) return;
	if (!OwnerCharacter || UDkAbilitySystemFunctionLibrary::IsActorInDialog(OwnerCharacter)) return;

	TaskNameText->SetText(CachedTaskNoticeList[0].TaskName);
	TaskNoticeStateText->SetText(
		UDkCommonFunctionLibrary::GetDisplayValueOfEnum(CachedTaskNoticeList[0].TaskNoticeState)
	);
	if (CachedTaskNoticeList[0].TaskNoticeState == ETaskNoticeState::NewTask)
	{
		UGameplayStatics::PlaySound2D(this, NewTaskSound);
	}
	else if (CachedTaskNoticeList[0].TaskNoticeState == ETaskNoticeState::TaskUpdate)
	{
		UGameplayStatics::PlaySound2D(this, TaskUpdateSound);
	}
	else if (CachedTaskNoticeList[0].TaskNoticeState == ETaskNoticeState::TaskCompleted)
	{
		UGameplayStatics::PlaySound2D(this, TaskCompletedSound);
	}
	PlayAnimationForward(ShowNoticeAnimation);

	CachedTaskNoticeList.RemoveAt(0);
	if (CachedTaskNoticeList.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(TaskNoticeTimerHandle);
	}
}
