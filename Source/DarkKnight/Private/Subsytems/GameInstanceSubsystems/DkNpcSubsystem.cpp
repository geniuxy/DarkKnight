// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/GameInstanceSubsystems/DkNpcSubsystem.h"

#include "DkTypes/DkStructs.h"
#include "Settings/DeveloperSettings/DkDataDeveloperSetting.h"

UDkNpcSubsystem* UDkNpcSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UDkNpcSubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

void UDkNpcSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	InitializeData();
}

void UDkNpcSubsystem::InitializeData()
{
	InitializeNpcInfo();
}

void UDkNpcSubsystem::InitializeNpcInfo()
{
	CachedNpcInfoMap.Empty();
	const UDkDataDeveloperSetting* DataDeveloperSettings = GetDefault<UDkDataDeveloperSetting>();
	if (UDataTable* NpcInfoDataTable = DataDeveloperSettings->GetNpcInfoDataTable())
	{
		for (FName RowName : NpcInfoDataTable->GetRowNames())
		{
			FNpcInfo* NpcInfo = NpcInfoDataTable->FindRow<FNpcInfo>(RowName, TEXT("没找到RowName对应的Row"));
			if (!CachedNpcInfoMap.Contains(NpcInfo->NpcId))
			{
				CachedNpcInfoMap.Add(NpcInfo->NpcId, *NpcInfo);
			}
		}
	}
}

void UDkNpcSubsystem::UpdateNpcInfo(int InNpcId, AActor* InNpcActor)
{
	for (TTuple<int, FNpcInfo>& NpcInfoPair : CachedNpcInfoMap)
	{
		if (NpcInfoPair.Key == InNpcId)
		{
			NpcInfoPair.Value.NpcActor = InNpcActor;
			break;
		}
	}
}
