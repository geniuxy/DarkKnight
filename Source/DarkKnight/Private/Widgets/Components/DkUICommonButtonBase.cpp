// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DkUICommonButtonBase.h"
#include "CommonTextBlock.h"
#include "DkTypes/DkEnums.h"
#include "Subsytems/DkUISubsystem.h"

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

FText UDkUICommonButtonBase::GetButtonDisplayText() const
{
	if (CommonButtonTextBlock)
	{
		return CommonButtonTextBlock->GetText();
	}
	return FText();
}

void UDkUICommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText, ButtonDisplayTextJustification);
}

void UDkUICommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (CommonButtonTextBlock && GetCurrentTextStyleClass())
	{
		CommonButtonTextBlock->SetStyle(GetCurrentTextStyleClass());
	}
}

void UDkUICommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!ButtonDescriptionText.IsEmpty())
	{
		UDkUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
	}
}

void UDkUICommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	UDkUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
}
