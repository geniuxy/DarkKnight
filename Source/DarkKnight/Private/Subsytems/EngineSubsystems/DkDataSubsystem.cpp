// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"

#include "Settings/DeveloperSettings/DkDataDeveloperSetting.h"

UDkDataSubsystem* UDkDataSubsystem::Get()
{
	if (!GEngine) return nullptr;
	auto* Subsystem = GEngine->GetEngineSubsystem<UDkDataSubsystem>();
	ensureMsgf(Subsystem, TEXT("UDkDataSubsystem not available - called too early or after shutdown"));
	return Subsystem;
}

void UDkDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDkDataSubsystem::InitializeDialogContent()
{
	CachedDialogContentMap.Empty();
	const UDkDataDeveloperSetting* DataDeveloperSettings = GetDefault<UDkDataDeveloperSetting>();
	if (UDataTable* DialogContentDataTable = DataDeveloperSettings->GetDialogContentDataTable())
	{
		for (FName RowName : DialogContentDataTable->GetRowNames())
		{
			FDialogContent* ContentInfo = DialogContentDataTable->FindRow<FDialogContent>(
				RowName, TEXT("没找到RowName对应的Row")
			);
			if (!CachedDialogContentMap.Contains(ContentInfo->Id))
			{
				CachedDialogContentMap.Add(ContentInfo->Id, *ContentInfo);
			}
		}
	}
}

void UDkDataSubsystem::InitializeNpcInfo()
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

void UDkDataSubsystem::UpdateNpcInfo(int InNpcId, AActor* InNpcActor)
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

void UDkDataSubsystem::InitializeTaskInfo()
{
	CachedTaskInfoMap.Empty();
	const UDkDataDeveloperSetting* DataDeveloperSettings = GetDefault<UDkDataDeveloperSetting>();
	if (UDataTable* TaskInfoDataTable = DataDeveloperSettings->GetTaskInfoDataTable())
	{
		for (FName RowName : TaskInfoDataTable->GetRowNames())
		{
			FTaskInfo* TaskInfo = TaskInfoDataTable->FindRow<FTaskInfo>(RowName, TEXT("没找到RowName对应的Row"));
			if (!CachedTaskInfoMap.Contains(TaskInfo->TaskId))
			{
				CachedTaskInfoMap.Add(TaskInfo->TaskId, *TaskInfo);
			}
		}
	}
}

void UDkDataSubsystem::InitializeEnemyTaskInfo()
{
	CachedEnemyTaskMap.Empty();
	const UDkDataDeveloperSetting* DataDeveloperSettings = GetDefault<UDkDataDeveloperSetting>();
	if (UDataTable* EnemyTaskDataTable = DataDeveloperSettings->GetEnemyTaskDataTable())
	{
		for (FName RowName : EnemyTaskDataTable->GetRowNames())
		{
			FEnemyTaskInfo* EnemyTaskInfo = EnemyTaskDataTable->FindRow<FEnemyTaskInfo>(RowName, TEXT("没找到RowName对应的Row"));
			if (!CachedEnemyTaskMap.Contains(EnemyTaskInfo->EnemyTag))
			{
				CachedEnemyTaskMap.Add(EnemyTaskInfo->EnemyTag, EnemyTaskInfo->CommitTaskDetails);
			}
		}
	}
}
