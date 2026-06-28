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
};
