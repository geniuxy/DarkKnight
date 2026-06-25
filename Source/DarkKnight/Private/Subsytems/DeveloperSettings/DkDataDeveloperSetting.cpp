// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/DeveloperSettings/DkDataDeveloperSetting.h"

UDataTable* UDkDataDeveloperSetting::GetDialogContentDataTable() const
{
	if (!CachedDialogContentDataTable.IsValid() && !DialogContentDataTable.IsNull())
	{
		CachedDialogContentDataTable = DialogContentDataTable.LoadSynchronous();
	}
	return CachedDialogContentDataTable.Get();
}
