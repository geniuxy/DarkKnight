// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/DkUIWidgetListEntryBase.h"

#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectBase.h"

void UDkUIWidgetListEntryBase::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, IsListItemSelected());
}

void UDkUIWidgetListEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UDkUIListDataObjectBase>(ListItemObject));
}

void UDkUIWidgetListEntryBase::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	NativeOnListEntryWidgetHovered(false);
}

FReply UDkUIWidgetListEntryBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	if (CommonInputSubsystem && CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* WidgetToFocus = NativeGetWidgetToFocusForGamepad())
		{
			if (TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget())
			{
				return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
			}
		}
	}

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

UWidget* UDkUIWidgetListEntryBase::NativeGetWidgetToFocusForGamepad() const
{
	return BP_GetWidgetToFocusForGamepad();
}

void UDkUIWidgetListEntryBase::OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject)
{
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}

	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}

	if (!InOwningListDataObject->OnDependencyDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->
			OnDependencyDataModified.AddUObject(this, &ThisClass::OnOwningDependencyDataObjectModified);
	}

	OnToggleEditableState(InOwningListDataObject->IsDataCurrentlyEditable());

	CachedOwningDataObject = InOwningListDataObject;
}

void UDkUIWidgetListEntryBase::OnOwningListDataObjectModified(
	UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
}

void UDkUIWidgetListEntryBase::OnOwningDependencyDataObjectModified(
	UDkUIListDataObjectBase* OwningModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningDataObject)
	{
		OnToggleEditableState(CachedOwningDataObject->IsDataCurrentlyEditable());
	}
}

void UDkUIWidgetListEntryBase::OnToggleEditableState(bool bIsEditable)
{
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetIsEnabled(bIsEditable);
	}
}

void UDkUIWidgetListEntryBase::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}
