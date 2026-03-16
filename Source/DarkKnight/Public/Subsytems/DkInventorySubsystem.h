// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkStructs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DkInventorySubsystem.generated.h"

class UDkInventoryComponent;
/**
 * UGameInstanceSubsystem 本质上不适用于网络同步场景
 * 需要网络同步时：AGameState全局游戏状态 / APlayerState玩家相关状态
 */
UCLASS()
class DARKKNIGHT_API UDkInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UDkInventorySubsystem* Get(const UObject* WorldContextObject);

	//~Begin USubsystem Function
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~End USubsystem Function

	UFUNCTION(BlueprintCallable)
	void RegisterCachedInventoryComponent(UDkInventoryComponent* InventoryComponent);

	void InitializeItemData();

	void InitializeEntryData();

private:
	UPROPERTY(Transient) // 不保存到磁盘，不参与网络复制
	UDkInventoryComponent* CachedInventoryComponent;

	UPROPERTY(Transient)
	TMap<int, FDkItemInfo> CachedItemTable;

	UPROPERTY(Transient)
	TMap<FName, FDkEntryInfo> CachedEntryTable;

public:
	LIST_DATA_ACCESSOR(UDkInventoryComponent*, CachedInventoryComponent)

	// 返回TMap的引用比较好，不容易指针悬空
	FORCEINLINE const TMap<int, FDkItemInfo>& GetCachedItemTable() const { return CachedItemTable; }
	FORCEINLINE const TMap<FName, FDkEntryInfo>& GetCachedEntryTable() const { return CachedEntryTable; }
};
