// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/DeveloperSettings/DkDataDeveloperSetting.h"

UDataTable* UDkDataDeveloperSetting::GetDialogContentDataTable() const
{
	if (!CachedDialogContentDataTable.IsValid() && !DialogContentDataTable.IsNull())
	{
		CachedDialogContentDataTable = DialogContentDataTable.LoadSynchronous();
	}
	return CachedDialogContentDataTable.Get();
}

UDataTable* UDkDataDeveloperSetting::GetNpcInfoDataTable() const
{
	if (!CachedNpcInfoDataTable.IsValid() && !NpcInfoDataTable.IsNull())
	{
		CachedNpcInfoDataTable = NpcInfoDataTable.LoadSynchronous();
	}
	return CachedNpcInfoDataTable.Get();
}
