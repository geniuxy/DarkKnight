// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Interact/DkWidgetDialogScreen.h"

#include "CommonTextBlock.h"
#include "DarkKnightDebugHelper.h"
#include "Characters/DkCharacterHero.h"
#include "Components/AudioComponent.h"
#include "Components/DkPlayerDialogComponent.h"
#include "Components/VerticalBox.h"
#include "Controllers/DkGamePlayerController.h"
#include "DkTypes/DkStructs.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Widgets/Components/Buttons/DkUICommonButtonImage.h"
#include "Widgets/Components/Buttons/Dialog/DkUIDialogSelectionButton.h"

void UDkWidgetDialogScreen::BeginDialog(int InStartDialogId)
{
	CurDialogId = InStartDialogId;
	CurDialogContent = GetDialogInfoById(InStartDialogId);
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

	// 更改NPC位置和镜头等

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

	SelectionList->ClearChildren();
	bCanClickToNextDialog = CurDialogContent.ContentType == EDialogContentType::Base;
	DialogConfirmButton->SetVisibility(bCanClickToNextDialog ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (CurDialogContent.ContentType == EDialogContentType::Branch)
	{
		for (TTuple<int, FDialogBranchInfo> BranchContentPair : CurDialogContent.BranchContents)
		{
			if (BranchContentPair.Value.Precondition.IsValid() && IsValid(OwnerDialogComponent))
			{
				bool bFinishPrecondition =
					OwnerDialogComponent->FindDialogGameplayTag(BranchContentPair.Value.Precondition);
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
	DeactivateWidget();
}

void UDkWidgetDialogScreen::JumpToBranchNextDialog(int InDialogId)
{
	CurDialogId = InDialogId;
	if (CurDialogId == 0)
	{
		EndDialog();
		return;
	}

	CurDialogContent = GetDialogInfoById(CurDialogId);
	UpdateDialogContent();
}
