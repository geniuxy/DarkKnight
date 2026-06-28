// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"

#include "Subsytems/DeveloperSettings/DkDataDeveloperSetting.h"

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
