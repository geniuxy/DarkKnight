// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkUIWidgetOptionDetailsView.h"

#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectBase.h"

void UDkUIWidgetOptionDetailsView::UpdateDetailsViewInfo(
	UDkUIListDataObjectBase* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject)
	{
		return;
	}

	CommonTextBlock_Title->SetText(InDataObject->GetDataDisplayName());

	if (!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(InDataObject->GetSoftDescriptionImage());
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	CommonRichText_Description->SetText(InDataObject->GetDescriptionRichText());

	const FString DynamicDetails = FString::Printf(
		TEXT("<Bold>Data Object类名: </>%s\n\n<Bold>Entry Widget类名: </>%s"),
		*InDataObject->GetClass()->GetName(),
		*InEntryWidgetClassName
	);

	CommonRichText_DynamicDetails->SetText(FText::FromString(DynamicDetails));

	CommonRichText_Warnings->SetText(InDataObject->GetWarningRichText());
}

void UDkUIWidgetOptionDetailsView::ClearDetailsViewInfo()
{
	CommonTextBlock_Title->SetText(FText::GetEmpty());
	CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	CommonRichText_Description->SetText(FText::GetEmpty());
	CommonRichText_DynamicDetails->SetText(FText::GetEmpty());
	CommonRichText_Warnings->SetText(FText::GetEmpty());
}

void UDkUIWidgetOptionDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetailsViewInfo();
}
