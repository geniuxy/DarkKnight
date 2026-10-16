// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"

#include "DarkKnightDebugHelper.h"
#include "GameplayEffect.h"
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
	InitializeEntryEffectData();
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

void UDkInventorySubsystem::InitializeEntryEffectData()
{
	CachedEntryEffectTable.Empty();

	const UDkInventoryDeveloperSettings* InventorySettings = GetDefault<UDkInventoryDeveloperSettings>();
	UDataTable* EntryEffectDataTable = InventorySettings->GetEntryEffectTable();
	checkf(EntryEffectDataTable, TEXT("词条Effect信息表(DT_EntryEffectInfo)还没有配置！"));

	// 行结构对不上时 FindRow 只会失败，先明确报出来，免得后面拿到一堆空数据还找不到原因
	if (EntryEffectDataTable->GetRowStruct() != FDkGameplayEffectInfo::StaticStruct())
	{
		Debug::Print(FString::Printf(
			TEXT("[%s]的RowStruct不是FDkGameplayEffectInfo，无法读取词条Effect信息！"),
			*EntryEffectDataTable->GetName()
		));
		return;
	}

	TArray<FName> RowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(EntryEffectDataTable, RowNames);
	for (const FName& RowName : RowNames)
	{
		const FDkGameplayEffectInfo* EffectInfo =
			EntryEffectDataTable->FindRow<FDkGameplayEffectInfo>(RowName, TEXT(""));
		if (!EffectInfo)
		{
			continue;
		}
		if (!EffectInfo->GameplayEffectTag.IsValid())
		{
			Debug::Print(FString::Printf(TEXT("词条Effect表的行[%s]没有配GameplayEffectTag，已忽略"), *RowName.ToString()));
			continue;
		}
		if (!IsValid(EffectInfo->Attribute))
		{
			Debug::Print(FString::Printf(TEXT("词条Effect表的行[%s]没有配Effect，已忽略"), *RowName.ToString()));
			continue;
		}

		// 同一个Tag配了多行时以先读到的为准，重复配置在这里报出来
		if (CachedEntryEffectTable.Contains(EffectInfo->GameplayEffectTag))
		{
			Debug::Print(FString::Printf(
				TEXT("词条Tag[%s]在词条Effect表里重复配置了，请检查！"), *EffectInfo->GameplayEffectTag.ToString()
			));
			continue;
		}
		CachedEntryEffectTable.Add(EffectInfo->GameplayEffectTag, EffectInfo->Attribute);
	}

	// 兜底校验：DT_DkEntryList 里用到的词条Tag必须都能在这里找到Effect。
	// GE类从 FDkEntryInfo 移到本表之后，漏配不会再有任何编译期提示，只能靠这里在开局报出来
	for (const TPair<FName, FDkEntryInfo>& Pair : CachedEntryTable)
	{
		const FGameplayTag& EntryTag = Pair.Value.GameplayEffectTag;
		if (EntryTag.IsValid() && !CachedEntryEffectTable.Contains(EntryTag))
		{
			Debug::Print(FString::Printf(
				TEXT("词条[%s]的Tag[%s]在词条Effect表里找不到对应Effect，该词条不会生效！"),
				*Pair.Key.ToString(), *EntryTag.ToString()
			));
		}
	}
}

TSubclassOf<UGameplayEffect> UDkInventorySubsystem::FindEntryEffectClass(const FGameplayTag& EntryTag) const
{
	if (!EntryTag.IsValid())
	{
		return nullptr;
	}

	if (const TSubclassOf<UGameplayEffect>* FoundClass = CachedEntryEffectTable.Find(EntryTag))
	{
		return *FoundClass;
	}
	return nullptr;
}
