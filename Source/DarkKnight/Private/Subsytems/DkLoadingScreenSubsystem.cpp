// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/DkLoadingScreenSubsystem.h"

#include "DarkKnightDebugHelper.h"

bool UDkLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance()) // 因为 UI 子系统通常不需要在服务器上运行
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses); // 获取当前类的所有派生类（子类）

		return FoundClasses.IsEmpty(); // 我不是专用服务器，而且没人继承我，我才允许自己被创建。
	}

	return false;
}

void UDkLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void UDkLoadingScreenSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDkLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	Debug::Print(TEXT("On Map Pre Loaded"));
}

void UDkLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	Debug::Print(TEXT("On Map Post Loaded"));
}
