// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"

#include "Kismet/DataTableFunctionLibrary.h"
#include "Settings/DkInventoryDeveloperSettings.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"

UDkInventorySubsystem* UDkInventorySubsystem::Get()
{
	if (!GEngine) return nullptr;
	auto* Subsystem = GEngine->GetEngineSubsystem<UDkInventorySubsystem>();
	ensureMsgf(Subsystem, TEXT("UDkInventorySubsystem not available - called too early or after shutdown"));
	return Subsystem;
}

bool UDkInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// // 目前， InventorySubsystem先不需要在服务器上运行
	// if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	// {
	
	// 目前， InventorySubsystem先在服务器和客户端上运行
	TArray<UClass*> FoundClasses;
	GetDerivedClasses(GetClass(), FoundClasses); // 获取当前类的所有派生类（子类）

	return FoundClasses.IsEmpty(); // 我不是专用服务器，而且没人继承我，我才允许自己被创建。
	
	// }
	//
	// return false;
}

void UDkInventorySubsystem::InitializeData()
{
	InitializeItemData();
	InitializeEntryData();
}

void UDkInventorySubsystem::InitializeItemData()
{
	CachedItemTable.Empty();
	const UDkInventoryDeveloperSettings* UIDeveloperSettings = GetDefault<UDkInventoryDeveloperSettings>();
	const UDataTable* ItemDataTable = UIDeveloperSettings->GetItemDataTable();
	checkf(ItemDataTable, TEXT("整体Item信息表还没有配置！"));
	TArray<FName> ItemDataTableRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(ItemDataTable, ItemDataTableRowNames);

	for (FName RowName : ItemDataTableRowNames)
	{
		FDkItemInfo* ItemInfo = ItemDataTable->FindRow<FDkItemInfo>(RowName, TEXT("没找到RowName对应的Row"));
		if (!CachedItemTable.Contains(ItemInfo->ItemID))
		{
			CachedItemTable.Add(ItemInfo->ItemID, *ItemInfo);
		}
	}
}

void UDkInventorySubsystem::InitializeEntryData()
{
	CachedEntryTable.Empty();
	const UDkInventoryDeveloperSettings* UIDeveloperSettings = GetDefault<UDkInventoryDeveloperSettings>();
	const UDataTable* EntryTable = UIDeveloperSettings->GetEntryTable();
	checkf(EntryTable, TEXT("整体Entry信息表还没有配置！"));
	TArray<FName> EntryDataTableRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(EntryTable, EntryDataTableRowNames);

	for (FName RowName : EntryDataTableRowNames)
	{
		FDkEntryInfo* EntryInfo = EntryTable->FindRow<FDkEntryInfo>(RowName, TEXT("没找到RowName对应的Row"));
		if (!CachedEntryTable.Contains(EntryInfo->EntryID))
		{
			CachedEntryTable.Add(EntryInfo->EntryID, *EntryInfo);
		}
	}
}
