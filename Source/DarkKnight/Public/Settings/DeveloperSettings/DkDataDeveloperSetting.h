// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DkDataDeveloperSetting.generated.h"

/**
 * DeveloperSetting的配置是支持每次PIE重启都更新内容的
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Data Settings"))
class DARKKNIGHT_API UDkDataDeveloperSetting : public UDeveloperSettings
{
	GENERATED_BODY()
	
	/**********************************************************************/
	/*                              Dialog                                */
	/**********************************************************************/
public:
	UPROPERTY(Config, EditAnywhere, Category = "Dialog")
	TSoftObjectPtr<UDataTable> DialogContentDataTable;

	// 获取加载后的 DataTable
	UDataTable* GetDialogContentDataTable() const;

private:
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedDialogContentDataTable;

	/**********************************************************************/
	/*                               Npc                                  */
	/**********************************************************************/
public:
	UPROPERTY(Config, EditAnywhere, Category = "Npc")
	TSoftObjectPtr<UDataTable> NpcInfoDataTable;

	// 获取加载后的 DataTable
	UDataTable* GetNpcInfoDataTable() const;

private:
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedNpcInfoDataTable;

	/**********************************************************************/
	/*                               Task                                 */
	/**********************************************************************/
public:
	UPROPERTY(Config, EditAnywhere, Category = "Task")
	TSoftObjectPtr<UDataTable> TaskInfoDataTable;

	UPROPERTY(Config, EditAnywhere, Category = "Task")
	TSoftObjectPtr<UDataTable> EnemyTaskDataTable;

	UPROPERTY(Config, EditAnywhere, Category = "Task")
	TSoftObjectPtr<UDataTable> TaskTrackingInfoDataTable;

	// 获取加载后的 DataTable
	UDataTable* GetTaskInfoDataTable() const;
	UDataTable* GetEnemyTaskDataTable() const;
	UDataTable* GetCachedTaskTrackingInfoDataTable() const;

private:
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedTaskInfoDataTable;

	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedEnemyTaskDataTable;

	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedTaskTrackingInfoDataTable;
};
