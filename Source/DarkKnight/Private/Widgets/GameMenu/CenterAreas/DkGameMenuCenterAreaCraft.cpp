// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/CenterAreas/DkGameMenuCenterAreaCraft.h"

#include "CommonListView.h"
#include "Widgets/Components/Buttons/Craft/DkCraftListData.h"


void UDkGameMenuCenterAreaCraft::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDkCraftListData* CategoryData = NewObject<UDkCraftListData>();
	CategoryData->CraftItemName = FText::FromString(TEXT("冰之哀伤"));
	CategoryData->IsReady = false;
	UDkCraftListData* CategoryData1 = NewObject<UDkCraftListData>();
	CategoryData1->CraftItemName = FText::FromString(TEXT("火之哀伤"));
	CategoryData1->IsReady = true;
	UDkCraftListData* CategoryData2 = NewObject<UDkCraftListData>();
	CategoryData2->CraftItemName = FText::FromString(TEXT("风之哀伤"));
	CategoryData2->IsReady = false;
	CraftItemListView->AddItem(CategoryData);
	CraftItemListView->AddItem(CategoryData1);
	CraftItemListView->AddItem(CategoryData2);
}
