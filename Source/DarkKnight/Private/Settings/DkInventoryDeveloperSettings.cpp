// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/DkInventoryDeveloperSettings.h"

UDataTable* UDkInventoryDeveloperSettings::GetItemDataTable() const
{
	if (!CachedItemTable.IsValid() && !ItemDataTable.IsNull())
	{
		CachedItemTable = ItemDataTable.LoadSynchronous();
	}
	return CachedItemTable.Get();
}

UDataTable* UDkInventoryDeveloperSettings::GetEntryTable() const
{
	if (!CachedEntryTable.IsValid() && !EntryTable.IsNull())
	{
		CachedEntryTable = EntryTable.LoadSynchronous();
	}
	return CachedEntryTable.Get();
}