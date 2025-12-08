// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DkUICommonButtonBase.h"
#include "CommonTextBlock.h"

void UDkUICommonButtonBase::SetButtonText(FText InText)
{
	if (CommonButtonTextBlock && !InText.IsEmpty())
	{
		CommonButtonTextBlock->SetText(bUseUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

void UDkUICommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}
