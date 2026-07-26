// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkStructs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DkInventorySubsystem.generated.h"

class UDkPlayerInventoryComp;
/**
 * UGameInstanceSubsystem 本质上不适用于网络同步场景，但是Client和Server上都存在，适合存放静态的数据
 * 需要网络同步时：AGameState全局游戏状态 / APlayerState玩家相关状态
 */
UCLASS()
class DARKKNIGHT_API UDkInventorySubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UDkInventorySubsystem* Get();

	//~Begin USubsystem Function
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~End USubsystem Function

	void InitializeData();

	void InitializeItemData();

	void InitializeEntryData();

private:
	UPROPERTY(Transient)
	TMap<int, FDkItemInfo> CachedItemTable;

	UPROPERTY(Transient)
	TMap<FName, FDkEntryInfo> CachedEntryTable;

public:
	// 返回TMap的引用比较好，不容易指针悬空
	FORCEINLINE const TMap<int, FDkItemInfo>& GetCachedItemTable() const { return CachedItemTable; }
	FORCEINLINE const TMap<FName, FDkEntryInfo>& GetCachedEntryTable() const { return CachedEntryTable; }
};
