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

UDataTable* UDkDataDeveloperSetting::GetTaskInfoDataTable() const
{
	if (!CachedTaskInfoDataTable.IsValid() && !TaskInfoDataTable.IsNull())
	{
		CachedTaskInfoDataTable = TaskInfoDataTable.LoadSynchronous();
	}
	return CachedTaskInfoDataTable.Get();
}

UDataTable* UDkDataDeveloperSetting::GetEnemyTaskDataTable() const
{
	if (!CachedEnemyTaskDataTable.IsValid() && !EnemyTaskDataTable.IsNull())
	{
		CachedEnemyTaskDataTable = EnemyTaskDataTable.LoadSynchronous();
	}
	return CachedEnemyTaskDataTable.Get();
}

UDataTable* UDkDataDeveloperSetting::GetCachedTaskTrackingInfoDataTable() const
{
	if (!CachedTaskTrackingInfoDataTable.IsValid() && !TaskTrackingInfoDataTable.IsNull())
	{
		CachedTaskTrackingInfoDataTable = TaskTrackingInfoDataTable.LoadSynchronous();
	}
	return CachedTaskTrackingInfoDataTable.Get();
}
