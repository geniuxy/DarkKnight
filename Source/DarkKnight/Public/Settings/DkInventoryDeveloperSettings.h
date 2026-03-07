// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DkInventoryDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "DarkKnight Inventory Settings"))
class DARKKNIGHT_API UDkInventoryDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Item Entry Tables")
	TSoftObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(Config, EditAnywhere, Category = "Item Entry Tables")
	TSoftObjectPtr<UDataTable> EntryTable;

	// 获取加载后的 DataTable
	UDataTable* GetItemDataTable() const;
	UDataTable* GetEntryTable() const;

private:
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedItemTable;
    
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedEntryTable;
};
