// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/Craft/DkUICraftReferenceButton.h"

#include "CommonLazyImage.h"
#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "Widgets/Components/Buttons/Craft/DkCraftListData.h"

void UDkUICraftReferenceButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	CraftListData = Cast<UDkCraftListData>(ListItemObject);
	if (CraftListData)
	{
		CraftItemIcon->SetBrushFromLazyTexture(CraftListData->CraftItemIcon);
		CraftItemNameText->SetText(CraftListData->CraftItemName);
		if (CraftListData->IsReady)
		{
			CraftMessageText->SetText(FText::FromString(TEXT("所需材料齐全")));
			CraftMessageText->SetColorAndOpacity(ReadyMessageColor);
		}
		else
		{
			CraftMessageText->SetText(FText::FromString(TEXT("部分材料缺失")));
			CraftMessageText->SetColorAndOpacity(NotReadyMessageColor);
		}
	}

	OwnerListView = Cast<UCommonListView>(GetOwningListView());
}

void UDkUICraftReferenceButton::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	HoverBorder->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UDkUICraftReferenceButton::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	HoverBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UDkUICraftReferenceButton::NativeOnClicked()
{
	Super::NativeOnClicked();
}
