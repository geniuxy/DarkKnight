// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Task/DkWidgetSubTaskTarget.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"

void UDkWidgetSubTaskTarget::ConfigureSubTaskTarget(
	bool IsFinished, const FText& TargetDescription, int CurSubTaskProgress, int TargetProgress)
{
	FinishedFlagImage->SetVisibility(IsFinished ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	FFormatNamedArguments Args;
	Args.Add(TEXT("A"), CurSubTaskProgress);
	Args.Add(TEXT("B"), TargetProgress);
	TargetDescriptionText->SetText(FText::Format(TargetDescription, Args));
}
