// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkNPCDialogComponent.h"

#include "DkGameplayTags.h"
#include "Controllers/DkGamePlayerController.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "Widgets/Interact/DkWidgetDialogScreen.h"


UDkNPCDialogComponent::UDkNPCDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InteractMessage = TEXT("F - 对话");
}

bool UDkNPCDialogComponent::CanStartDialog()
{
	if (NPCId == 0 || StartDialogId == 0) return false;
	return true;
}

void UDkNPCDialogComponent::TryStartDialog(ADkGamePlayerController* PC)
{
	UDkUISubsystem* UISubsystem = UDkUISubsystem::Get(this);
	checkf(UISubsystem, TEXT("UISubsystem为空！"));

	UISubsystem->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_Interact,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_DialogScreen),
		[=, this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				{
					PushedWidget->SetOwningPlayer(PC);
					break;
				}
			case EAsyncPushWidgetState::AfterPush:
				CachedDialogScreen = CastChecked<UDkWidgetDialogScreen>(PushedWidget);
				CachedDialogScreen->BeginDialog(StartDialogId);
				break;
			}
		}
	);
}

void UDkNPCDialogComponent::BeginPlay()
{
	Super::BeginPlay();
}

