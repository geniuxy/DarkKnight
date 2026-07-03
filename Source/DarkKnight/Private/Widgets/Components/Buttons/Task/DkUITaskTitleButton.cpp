// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/Task/DkUITaskTitleButton.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Components/Border.h"

void UDkUITaskTitleButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (TitleColor)
	{
		TitleColor->SetBrushColor(TaskTypeColor);
	}
	if (TitleTextBlock)
	{
		TitleTextBlock->SetText(TitleText);
	}
	if (TitleOpenStatus)
	{
		TitleOpenStatus->SetRenderTransformAngle(bOpen ? 90.f : 180.f);
	}
}

void UDkUITaskTitleButton::OnTaskTitleClicked()
{
	bOpen = !bOpen;

	if (TitleOpenStatus)
	{
		TitleOpenStatus->SetRenderTransformAngle(bOpen ? 90.f : 180.f);
	}
}
