// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryItemDescriptionMenu.h"

#include "CommonTextBlock.h"
#include "Components/SizeBox.h"

void UDkInventoryItemDescriptionMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(Text_ItemOptionalTitle))
	{
		Text_ItemOptionalTitle->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDkInventoryItemDescriptionMenu::ShowOptionalStatTitle() const
{
	if (!IsValid(Text_ItemOptionalTitle)) return;
	Text_ItemOptionalTitle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

FVector2D UDkInventoryItemDescriptionMenu::GetBoxSize() const
{
	return SizeBox_Root->GetDesiredSize();
}
