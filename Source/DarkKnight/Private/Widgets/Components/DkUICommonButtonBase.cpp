// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DkUICommonButtonBase.h"
#include "CommonTextBlock.h"

void UDkUICommonButtonBase::SetButtonText(FText InText, EDkTextJustify InJustification)
{
	if (CommonButtonTextBlock && !InText.IsEmpty())
	{
		CommonButtonTextBlock->SetText(bUseUpperCaseForButtonText ? InText.ToUpper() : InText);
		switch (InJustification)
		{
		case EDkTextJustify::Left:
			CommonButtonTextBlock->SetJustification(ETextJustify::Left);
			break;
		case EDkTextJustify::Center:
			CommonButtonTextBlock->SetJustification(ETextJustify::Center);
			break;
		case EDkTextJustify::Right:
			CommonButtonTextBlock->SetJustification(ETextJustify::Right);
			break;
		}
	}
}

void UDkUICommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText, ButtonDisplayTextJustification);
}
