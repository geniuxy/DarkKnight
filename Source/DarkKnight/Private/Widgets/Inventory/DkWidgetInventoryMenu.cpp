// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkWidgetInventoryMenu.h"

#include "CommonListView.h"
#include "Components/InventoryComps/DkInventoryComponent.h"
#include "Widgets/Inventory/DkInventoryItemGrid.h"
#include "Widgets/Inventory/Components/DkInventoryCategoryButton.h"

void UDkWidgetInventoryMenu::SetInventoryComponent(UDkInventoryComponent* InInventoryComp)
{
	InventoryComponent = InInventoryComp;

	InitInventoryCategoryButtons();
}

void UDkWidgetInventoryMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UDkWidgetInventoryMenu::InitInventoryCategoryButtons()
{
	if (!InventoryComponent.IsValid()) return;

	CategoryButtonListView->ClearListItems();
	for (const FInventoryItemCategoryInfo& CategoryInfo : InventoryComponent->GetAllInventoryCategoryInfo())
	{
		UInventoryItemCategoryData* CategoryData = NewObject<UInventoryItemCategoryData>();
		CategoryData->Info = CategoryInfo;
		CategoryButtonListView->AddItem(CategoryData);
	}

	CategoryButtonListView->OnItemSelectionChanged().AddUObject(this, &ThisClass::CategoryButtonPressed);

	// AddItem会创建Widget，要等下一次Tick再处理点击逻辑，不然会有空指针
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
		{
			CategoryButtonListView->SetSelectedIndex(0);
		})
	);
}

void UDkWidgetInventoryMenu::CategoryButtonPressed(UObject* SelectedUObject)
{
	if (const UInventoryItemCategoryData* CategoryData = Cast<UInventoryItemCategoryData>(SelectedUObject))
	{
		if (LastSelectedCategoryButton)
		{
			LastSelectedCategoryButton->ToggleHighlightState(false);
		}
		UDkInventoryCategoryButton* SelectedCategoryButton =
			CategoryButtonListView->GetEntryWidgetFromItem<UDkInventoryCategoryButton>(CategoryData);
		if (SelectedCategoryButton)
		{
			SelectedCategoryButton->ToggleHighlightState(true);
			LastSelectedCategoryButton = SelectedCategoryButton;
		}

		if (InventoryGrid)
		{
			InventoryGrid->ConstructGrid(CategoryData->Info.Rows, CategoryData->Info.Columns);
		}
	}
}
