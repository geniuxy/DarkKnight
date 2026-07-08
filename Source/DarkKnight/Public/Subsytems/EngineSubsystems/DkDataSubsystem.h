// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkStructs.h"
#include "Subsystems/EngineSubsystem.h"
#include "DkDataSubsystem.generated.h"

/**
 * UEngineSubsystem相比UGameInstanceSubsystem的好处：
 * 1. UEngineSubsystem 没有 World 依赖，CDO 上也能正常访问。
 * 2. 目前我只存放静态的数据
 * 缺点是：编辑器里修改了，必须重启一下才能测试到修改内容。(Initialize只在编辑器启动的时候执行一次！)
 */
UCLASS()
class DARKKNIGHT_API UDkDataSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UDkDataSubsystem* Get();

	//~Begin USubsystem Function
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~End USubsystem Function
	
	void InitializeData();
	/**********************************************************************/
	/*                              Dialog                                */
	/**********************************************************************/
public:
	void InitializeDialogContent();

private:
	UPROPERTY(Transient)
	TMap<int, FDialogContent> CachedDialogContentMap;

public:
	FORCEINLINE TMap<int, FDialogContent> GetDialogContentInfo() const { return CachedDialogContentMap; }

	/**********************************************************************/
	/*                                Npc                                 */
	/**********************************************************************/
public:
	void InitializeNpcInfo();

	void UpdateNpcInfo(int InNpcId, AActor* InNpcActor);

private:
	UPROPERTY(Transient)
	TMap<int, FNpcInfo> CachedNpcInfoMap;

public:
	FORCEINLINE TMap<int, FNpcInfo> GetNpcInfo() const { return CachedNpcInfoMap; }

	/**********************************************************************/
	/*                                Task                                */
	/**********************************************************************/
public:
	void InitializeTaskInfo();
	void InitializeEnemyTaskInfo();

private:
	UPROPERTY(Transient)
	TMap<int, FTaskInfo> CachedTaskInfoMap;

	UPROPERTY(Transient)
	TMap<FGameplayTag, FCommitTaskDetailsArray> CachedEnemyTaskMap;

public:
	FORCEINLINE TMap<int, FTaskInfo> GetTaskInfo() const { return CachedTaskInfoMap; }
	FORCEINLINE TMap<FGameplayTag, FCommitTaskDetailsArray> GetEnemyTaskMap() const { return CachedEnemyTaskMap; }
};
