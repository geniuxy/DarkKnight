// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Components/DkInventoryCategoryButton.h"

#include "CommonListView.h"
#include "Inventory/DkInventoryCategoryItems.h"
#include "Widgets/Components/Buttons/DkUICommonButtonImage.h"

void UDkInventoryCategoryButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	CategoryData = Cast<UInventoryItemCategoryData>(ListItemObject);
	if (CategoryButton && CategoryData)
	{
		CategoryButton->SetButtonDisplayImage(CategoryData->Info.CategoryIcon);
	}
	SelectedUnderline->SetVisibility(ESlateVisibility::Hidden);

	OwnerListView = Cast<UCommonListView>(GetOwningListView());

	CategoryButton->OnClicked().AddUObject(this, &ThisClass::HandleCategoryButtonClicked);
}

void UDkInventoryCategoryButton::ToggleHighlightState(bool bShouldHighlight)
{
	CategoryButton->ToggleHighlightState(bShouldHighlight);

	SelectedUnderline->SetVisibility(
		bShouldHighlight ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden
	);
}

void UDkInventoryCategoryButton::HandleCategoryButtonClicked()
{
	if (IsValid(OwnerListView) && IsValid(CategoryData))
	{
		OwnerListView->SetSelectedItem(CategoryData);
	}
}
