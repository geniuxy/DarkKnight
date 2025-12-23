// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/DkUIWidgetListEntryBase.h"

#include "CommonTextBlock.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectBase.h"

void UDkUIWidgetListEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UDkUIListDataObjectBase>(ListItemObject));
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
}

void UDkUIWidgetListEntryBase::OnOwningListDataObjectModified(
	UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
}
