// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/DkLoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
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
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

UWorld* UDkLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}
	return nullptr;
}

void UDkLoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType UDkLoadingScreenSubsystem::GetTickableTickType() const
{
	// CDO（Class Default Object）或任何“模板”对象在编辑器里也会走一遍这个函数，但它们并不是世界里的真正实例，Tick 它们没意义。
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}
	return ETickableTickType::Conditional; // Use IsTickable to determine whether to tick
}

bool UDkLoadingScreenSubsystem::IsTickable() const
{
	// 专用服务器（Dedicated Server）进程没有窗口、没有视口, 不需要tick
	// UGameViewportClient 是 UE 里“窗口+输入+分屏”三大功能的总入口；
	// Tick运行在带窗口的客户端
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UDkLoadingScreenSubsystem::GetStatId() const
{
	// GetStatId() 本身不“干”业务逻辑，只是向 UE 的性能监控器报个到
	// 固定写法
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFrontendLoadingScreenSubsystem, STATGROUP_Tickables);
}

void UDkLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}

	// 开启tick
	SetTickableTickType(ETickableTickType::Conditional);
	
	bIsCurrentlyLoadingMap = true;
	
	TryUpdateLoadingScreen();
}

void UDkLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}
}

void UDkLoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	// 检查目前是否有加载界面正在展示
	if (IsPreLoadScreenActive())
	{
		return;
	}

	// 检查是否应该展示加载界面
	if (true)
	{
		// 尝试展示加载界面
	}
	else
	{
		// 移除正在显示的加载界面

		// 通知加载已完成

		// 禁止Tick操作
		SetTickableTickType(ETickableTickType::Never);
	}
}

bool UDkLoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	if (FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
		return PreLoadScreenManager->HasValidActivePreLoadScreen();
	}
	return false;
}
