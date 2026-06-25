// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DkDataDeveloperSetting.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Data Settings"))
class DARKKNIGHT_API UDkDataDeveloperSetting : public UDeveloperSettings
{
	GENERATED_BODY()
	
	/**********************************************************************/
	/*                              Dialog                                */
	/**********************************************************************/
public:
	UPROPERTY(Config, EditAnywhere, Category = "GAS")
	TSoftObjectPtr<UDataTable> DialogContentDataTable;

	// 获取加载后的 DataTable
	UDataTable* GetDialogContentDataTable() const;

private:
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedDialogContentDataTable;
};
