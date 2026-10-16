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

	// 词条Tag → GameplayEffect 的映射表（行结构 FDkGameplayEffectInfo）。
	// 词条要施加哪个GE只在这里配置一处，DT_DkEntryList 与 Fragment 都不再各自存一份GE类
	UPROPERTY(Config, EditAnywhere, Category = "Item Entry Tables")
	TSoftObjectPtr<UDataTable> EntryEffectTable;

	// 获取加载后的 DataTable
	UDataTable* GetItemDataTable() const;
	UDataTable* GetEntryTable() const;
	UDataTable* GetEntryEffectTable() const;

private:
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedItemTable;
    
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedEntryTable;

	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UDataTable> CachedEntryEffectTable;
};
