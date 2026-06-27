// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Interact/DkWidgetDialogScreen.h"

#include "CommonTextBlock.h"
#include "Characters/DkCharacterHero.h"
#include "Components/DkPlayerDialogComponent.h"
#include "Components/VerticalBox.h"
#include "DkTypes/DkStructs.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Components/Buttons/DkUICommonButtonBase.h"
#include "Widgets/Components/Buttons/Dialog/DkUIDialogSelectionButton.h"

void UDkWidgetDialogScreen::UpdateDialogContent(FDialogContent CurrentDialogContent)
{
	// 更改NPC位置和镜头等

	DialogContent->SetText(CurrentDialogContent.ContentText);

	SelectContentList->ClearChildren();

	// 播放配音
	if (CurrentDialogContent.Dubbing)
	{
		UGameplayStatics::PlaySound2D(this, CurrentDialogContent.Dubbing);
	}

	bCanClickToNextDialog = CurrentDialogContent.ContentType == EDialogContentType::Base;
	if (CurrentDialogContent.ContentType == EDialogContentType::Branch)
	{
		for (TTuple<int, FDialogBranchInfo> BranchContentPair : CurrentDialogContent.BranchContents)
		{
			if (BranchContentPair.Value.Precondition.IsValid() && IsValid(OwnerDialogComponent))
			{
				bool bFinishPrecondition =
					OwnerDialogComponent->FindDialogGameplayTag(BranchContentPair.Value.Precondition);
				if (!bFinishPrecondition) continue;
			}

			UDkUIDialogSelectionButton* DialogSelectionButton = CreateWidget<UDkUIDialogSelectionButton>(
				this, DialogSelectionButtonClass
			);
			SelectContentList->AddChild(DialogSelectionButton);
		}
	}
}

void UDkWidgetDialogScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OwnerCharacter = Cast<ADkCharacterHero>(GetOwningPlayerPawn());
	if (OwnerCharacter)
	{
		OwnerDialogComponent = OwnerCharacter->GetPlayerDialogComponent();
	}
}

void UDkWidgetDialogScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly);
}

void UDkWidgetDialogScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);
}
