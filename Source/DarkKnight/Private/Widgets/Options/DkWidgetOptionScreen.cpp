// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkWidgetOptionScreen.h"

#include "DarkKnightDebugHelper.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"
#include "Settings/DkGameUserSettings.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/Components/DkUICommonButtonBase.h"
#include "Widgets/Components/DkUICommonListView.h"
#include "Widgets/Components/DkUITabListWidgetBase.h"
#include "Widgets/Options/DkUIOptionListEntryDataMapping.h"
#include "Widgets/Options/DkUIOptionsDataRegistry.h"
#include "Widgets/Options/DkUIWidgetOptionDetailsView.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectCollection.h"
#include "Widgets/Options/ListEntries/DkUIWidgetListEntryBase.h"

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
	// 用于切换Tab后，将每一个设置项都先置为Unhovered状态
	CommonListView_OptionsList->OnEntryWidgetReleased().AddUObject(this, &ThisClass::OnEntryWidgetReleased);
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
	if (ResettableDataArray.IsEmpty())
	{
		return;
	}

	UCommonButtonBase* SelectedTabButton =
		TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab());
	const FString SelectedTabButtonName =
		CastChecked<UDkUICommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();

	UDkUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesOrNo,
		FText::FromString(TEXT("重置")),
		FText::FromString(TEXT("你想要重置所有 ") + SelectedTabButtonName + TEXT(" 下的设置吗?")),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}

			bIsResettingData = true;
			bool bHasDataFailedToReset = false;

			for (UDkUIListDataObjectBase* DataToReset : ResettableDataArray)
			{
				if (!DataToReset)
				{
					continue;
				}

				if (DataToReset->TryResetBackToDefaultValue())
				{
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT("被重置了！"));
				}
				else
				{
					bHasDataFailedToReset = true;
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT("重置失败！"));
				}
			}

			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();
				RemoveActionBinding(ResetActionHandle);
			}

			bIsResettingData = false;
		}
	);
}

void UDkWidgetOptionScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UDkWidgetOptionScreen::OnOptionsTabSelected(FName TabId)
{
	DetailsView_ListEntryInfo->ClearDetailsViewInfo(); // 切换标签页时清空设置详情说明

	TArray<UDkUIListDataObjectBase*> FoundListSourceItems =
		GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);
	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();

	for (UDkUIListDataObjectBase* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewDataModified);
		}

		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UDkWidgetOptionScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem)
	{
		return;
	}

	UDkUIWidgetListEntryBase* HoveredEntryWidget =
		CommonListView_OptionsList->GetEntryWidgetFromItem<UDkUIWidgetListEntryBase>(InHoveredItem);

	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);
}

void UDkWidgetOptionScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}

	DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
		CastChecked<UDkUIListDataObjectBase>(InSelectedItem),
		TryGetEntryWidgetClassName(InSelectedItem)
	);
}

void UDkWidgetOptionScreen::OnEntryWidgetReleased(UUserWidget& InReleasedWidget)
{
	if (!IsValid(&InReleasedWidget))
	{
		return;
	}

	if (UDkUIWidgetListEntryBase* EntryWidget = Cast<UDkUIWidgetListEntryBase>(&InReleasedWidget))
	{
		EntryWidget->NativeOnListEntryWidgetHovered(false);
	}
}

void UDkWidgetOptionScreen::OnListViewDataModified(
	UDkUIListDataObjectBase* InModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (!InModifiedData || bIsResettingData)
	{
		return;
	}

	if (InModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(InModifiedData);

		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(InModifiedData))
		{
			ResettableDataArray.Remove(InModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}

FString UDkWidgetOptionScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if (UUserWidget* FoundEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}

	if (TSubclassOf<UDkUIWidgetListEntryBase> FoundEntryWidget = CommonListView_OptionsList->GetListEntryDataMapping()->
		FindEntryWidgetClassByDataObject(CastChecked<UDkUIListDataObjectBase>(InOwningListItem)))
	{
		return FoundEntryWidget->GetName();
	}

	return TEXT("无效的EntryWidget类名");
}
