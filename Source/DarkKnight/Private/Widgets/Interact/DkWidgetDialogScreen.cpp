// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Interact/DkWidgetDialogScreen.h"

#include "CommonTextBlock.h"
#include "DarkKnightDebugHelper.h"
#include "Characters/DkCharacterHero.h"
#include "Components/AudioComponent.h"
#include "Components/DkNpcDialogComponent.h"
#include "Components/DkPlayerDialogComponent.h"
#include "Components/VerticalBox.h"
#include "Controllers/DkGamePlayerController.h"
#include "DkTypes/DkStructs.h"
#include "FunctionLibrarys/DkDialogFunctionLibrary.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Widgets/Components/Buttons/DkUICommonButtonImage.h"
#include "Widgets/Components/Buttons/Dialog/DkUIDialogSelectionButton.h"

void UDkWidgetDialogScreen::BeginDialog(int InStartDialogId, UDkNpcDialogComponent* InNpcDialogComponent)
{
	LastDialogId = InStartDialogId;
	CurDialogId = InStartDialogId;
	CurDialogContent = GetDialogInfoById(InStartDialogId);
	NpcDialogComponent = InNpcDialogComponent;
	if (NpcDialogComponent)
	{
		NpcDialogComponent->CacheNpcTransform(CurDialogContent.NPCInfos);
	}

	UpdateDialogContent();
}

void UDkWidgetDialogScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OwnerCharacter = Cast<ADkCharacterHero>(GetOwningPlayerPawn());
	if (OwnerCharacter)
	{
		OwnerDialogComponent = OwnerCharacter->GetPlayerDialogComponent();
	}
	OwnerPC = Cast<ADkGamePlayerController>(GetOwningPlayer());

	DialogConfirmButton->OnClicked().AddUObject(this, &ThisClass::DialogConfirmButtonClicked);
}

void UDkWidgetDialogScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly, false);
}

void UDkWidgetDialogScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);
}

void UDkWidgetDialogScreen::DialogConfirmButtonClicked()
{
	if (!bCanClickToNextDialog) return;
	LastDialogId = CurDialogId;
	CurDialogId = GetNextDialogId();
	if (CurDialogId == 0)
	{
		EndDialog();
		return;
	}

	CurDialogContent = GetDialogInfoById(CurDialogId);
	UpdateDialogContent();
}

void UDkWidgetDialogScreen::UpdateDialogContent()
{
	if (CurDialogContent.Id == 0) return;

	// 围绕主Npc, 更改NPC位置
	if (NpcDialogComponent && !CurDialogContent.NPCInfos.IsEmpty())
	{
		NpcDialogComponent->UpdateNpcTransform(CurDialogContent.NPCInfos);
	}
	// 更改NPC镜头
	if (OwnerDialogComponent)
	{
		OwnerDialogComponent->UpdateCameraFocus(
			CurDialogContent.CameraFocusNpcId, CurDialogContent.CameraType, CurDialogContent.CustomCameraTransform
		);
	}

	// 显示对话内容
	DialogContentText->SetText(CurDialogContent.ContentText);

	// 播放配音
	if (CurDialogContent.Dubbing)
	{
		if (!IsValid(CachedAudioComponent))
		{
			CachedAudioComponent = UGameplayStatics::CreateSound2D(this, CurDialogContent.Dubbing);
		}
		CachedAudioComponent->SetSound(CurDialogContent.Dubbing);
		CachedAudioComponent->Play();
	}

	// 下一个对话点击按钮是否可点击
	bCanClickToNextDialog = CurDialogContent.ContentType == EDialogContentType::Base;
	DialogConfirmButton->SetVisibility(bCanClickToNextDialog ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	// 显示分支(如果有的话)
	SelectionList->ClearChildren();
	if (CurDialogContent.ContentType == EDialogContentType::Branch)
	{
		for (TTuple<int, FDialogBranchInfo> BranchContentPair : CurDialogContent.BranchContents)
		{
			if (BranchContentPair.Value.Preconditions.IsValid() && IsValid(OwnerDialogComponent))
			{
				bool bFinishPrecondition =
					OwnerDialogComponent->FindDialogGameplayTag(BranchContentPair.Value.Preconditions);
				if (!bFinishPrecondition) continue;
			}
			checkf(IsValid(DialogSelectionButtonClass), TEXT("DialogSelectionButtonClass没有配置"));

			UDkUIDialogSelectionButton* DialogSelectionButton = CreateWidget<UDkUIDialogSelectionButton>(
				this, DialogSelectionButtonClass
			);
			if (IsValid(OwnerDialogComponent))
			{
				DialogSelectionButton->ConfigureDialogSelectionButton(BranchContentPair.Value, OwnerDialogComponent);
			}
			SelectionList->AddChild(DialogSelectionButton);
			DialogSelectionButton->OnSelectionButtonClicked.AddUObject(this, &ThisClass::JumpToBranchNextDialog);
		}
	}
}

FDialogContent UDkWidgetDialogScreen::GetDialogInfoById(int InDialogId)
{
	TMap<int, FDialogContent> DialogContentInfoMap = UDkDataSubsystem::Get()->GetDialogContentInfo();
	if (DialogContentInfoMap.Contains(InDialogId))
	{
		return DialogContentInfoMap.FindRef(InDialogId);
	}

	Debug::Print("DialogContentDataTable中找不到Id对应的内容");
	EndDialog();
	return FDialogContent();
}

int UDkWidgetDialogScreen::GetNextDialogId()
{
	return CurDialogContent.NextContentId;
}

void UDkWidgetDialogScreen::EndDialog()
{
	bCanClickToNextDialog = false;
	if (OwnerPC)
	{
		OwnerPC->EndDialog();
	}
	CachedAudioComponent->StopDelayed(0.5f);

	FTimerHandle DeactivateHandle;
	GetWorld()->GetTimerManager().SetTimer(DeactivateHandle, FTimerDelegate::CreateLambda([this]()
	{
		DeactivateWidget();
		if (NpcDialogComponent)
		{
			NpcDialogComponent->ResetNpcTransform();
		}

		if (OwnerDialogComponent)
		{
			OwnerDialogComponent->UpdateCameraFocus(0, EDialogCameraType::None, FTransform());
		}
	}), 0.5f, false);

	// 结束对话后，如果有任务要更新的话，需要更新
	if (CurDialogId == 0 && UDkDialogFunctionLibrary::HasTaskToCommit(LastDialogId))
	{
		FCommitTaskDetails CommitTaskDetails = UDkDialogFunctionLibrary::GetDialogCommitTaskDetails(LastDialogId);
		if (ADkPlayerStateBase* OwnerPlayerState = OwnerCharacter->GetPlayerState<ADkPlayerStateBase>())
		{
			OwnerPlayerState->OnCommitTaskDelegate.Broadcast(
				CommitTaskDetails.MainTaskId,
				CommitTaskDetails.SubTaskId,
				CommitTaskDetails.CommitCount
			);
		}
	}
}

void UDkWidgetDialogScreen::JumpToBranchNextDialog(int InDialogId)
{
	LastDialogId = CurDialogId;
	CurDialogId = InDialogId;
	if (CurDialogId == 0)
	{
		EndDialog();
		return;
	}

	CurDialogContent = GetDialogInfoById(CurDialogId);
	UpdateDialogContent();
}
