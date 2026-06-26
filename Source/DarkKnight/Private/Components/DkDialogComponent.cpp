// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkDialogComponent.h"

#include "DkGameplayTags.h"
#include "Controllers/DkGamePlayerController.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "Widgets/Interact/DkWidgetDialogScreen.h"


UDkDialogComponent::UDkDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

bool UDkDialogComponent::CanStartDialog()
{
	if (NPCId == 0 || DialogId == 0) return false;
	return true;
}

void UDkDialogComponent::TryStartDialog(ADkGamePlayerController* PC)
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
				break;
			}
		}
	);
}

void UDkDialogComponent::BeginPlay()
{
	Super::BeginPlay();
}

