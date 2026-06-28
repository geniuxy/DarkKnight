// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/Dialog/DkUIDialogSelectionButton.h"

#include "CommonTextBlock.h"
#include "Components/DkPlayerDialogComponent.h"
#include "DkTypes/DkStructs.h"

void UDkUIDialogSelectionButton::ConfigureDialogSelectionButton(
	const FDialogBranchInfo& InBranchInfo, UDkPlayerDialogComponent* InDialogComp)
{
	CurBranchInfo = InBranchInfo;
	OwnerDialogComponent = InDialogComp;

	if (!CommonButtonTextBlock) return;
	SetButtonText(CurBranchInfo.BranchText);
	SetIsEnabled(true);
	if (HasFinishedBranch())
	{
		CommonButtonTextBlock->SetColorAndOpacity(FinishedBranchTextColor);
		SetIsEnabled(false);
	}

	CommonLazyImage_ButtonImage->SetVisibility(ESlateVisibility::Hidden);
}

void UDkUIDialogSelectionButton::NativeOnClicked()
{
	Super::NativeOnClicked();

	if (!IsValid(OwnerDialogComponent)) return;
	if (!HasFinishedBranch() && CurBranchInfo.AddDialogTag.IsValid())
	{
		OwnerDialogComponent->AddDialogTag(CurBranchInfo.AddDialogTag);
	}

	OnSelectionButtonClicked.Broadcast(CurBranchInfo.JumpToContentId);
}

void UDkUIDialogSelectionButton::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (CommonLazyImage_ButtonImage)
	{
		CommonLazyImage_ButtonImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UDkUIDialogSelectionButton::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (CommonLazyImage_ButtonImage)
	{
		CommonLazyImage_ButtonImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UDkUIDialogSelectionButton::HasFinishedBranch()
{
	if (!CurBranchInfo.AddDialogTag.IsValid()) return false;
	return OwnerDialogComponent &&
		OwnerDialogComponent->FindDialogGameplayTag(FGameplayTagContainer(CurBranchInfo.AddDialogTag));
}
