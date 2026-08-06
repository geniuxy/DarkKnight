// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkStructs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DkNpcSubsystem.generated.h"

class UDkNpcDialogComponent;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkNpcSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UDkNpcSubsystem* Get(const UObject* WorldContextObject);

	//~Begin USubsystem Function
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~End USubsystem Function

	void InitializeData();

	/**********************************************************************/
	/*                                Npc                                 */
	/**********************************************************************/
public:
	void InitializeNpcInfo();
	
	void UpdateNpcInfo(int InNpcId, AActor* InNpcActor);

	void SetCurNpcDialogComp(UDkNpcDialogComponent* InNpcDialogComp) { CachedNpcDialogComp = InNpcDialogComp; }

private:
	UPROPERTY(Transient)
	TMap<int, FNpcInfo> CachedNpcInfoMap;

	UPROPERTY(Transient)
	UDkNpcDialogComponent* CachedNpcDialogComp;

public:
	FORCEINLINE TMap<int, FNpcInfo> GetNpcInfo() const { return CachedNpcInfoMap; }
	FORCEINLINE UDkNpcDialogComponent* GetCachedNpcDialogComp() const { return CachedNpcDialogComp; }
};
