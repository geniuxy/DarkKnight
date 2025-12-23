// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/DkUIListDataObjectBase.h"

#include "Settings/DkGameUserSettings.h"

void UDkUIListDataObjectBase::InitDataObject()
{
	OnDataObjectInitialized();
}

void UDkUIListDataObjectBase::OnDataObjectInitialized()
{
}

void UDkUIListDataObjectBase::NotifyListDataModified(
	UDkUIListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmediately)
	{
		UDkGameUserSettings::Get()->ApplySettings(true);
	}
}
