// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkWidgetOptionScreen.h"

#include "DarkKnightDebugHelper.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"
#include "Settings/DkGameUserSettings.h"
#include "Widgets/Components/DkUICommonListView.h"
#include "Widgets/Components/DkUITabListWidgetBase.h"
#include "Widgets/Options/DkUIOptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectCollection.h"

void UDkWidgetOptionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
		);
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);

	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);

	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
}

void UDkWidgetOptionScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UDkUIListDataObjectCollection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
		{
			continue;
		}

		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabCollection->GetDataID()) != nullptr)
		{
			continue;
		}

		TabListWidget_OptionsTabs->RequestRegisterTab(TabCollection->GetDataID(), TabCollection->GetDataDisplayName());
	}
}

void UDkWidgetOptionScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	// 将更改写入config文件中
	UDkGameUserSettings::Get()->ApplySettings(true);
}

UDkUIOptionsDataRegistry* UDkWidgetOptionScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UDkUIOptionsDataRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegister(GetOwningLocalPlayer());
	}

	checkf(CreatedOwningDataRegistry, TEXT("OptionScreen的OptionsDataRegistry无效"));
	return CreatedOwningDataRegistry;
}

void UDkWidgetOptionScreen::OnResetBoundActionTriggered()
{
	Debug::Print(TEXT("重置绑定按钮已触发"));
}

void UDkWidgetOptionScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UDkWidgetOptionScreen::OnOptionsTabSelected(FName TabId)
{
	TArray<UDkUIListDataObjectBase*> FoundListSourceItems =
		GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);
	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}
}

void UDkWidgetOptionScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem)
	{
		return;
	}

	const FString DebugString = CastChecked<UDkUIListDataObjectBase>(InHoveredItem)->GetDataDisplayName().ToString() +
		TEXT(" was ") + (bWasHovered ? TEXT("hovered") : TEXT("unhovered"));

	Debug::Print(DebugString);
}

void UDkWidgetOptionScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}

	const FString DebugString = CastChecked<UDkUIListDataObjectBase>(InSelectedItem)->GetDataDisplayName().ToString() +
		TEXT(" was selected");

	Debug::Print(DebugString);
}
