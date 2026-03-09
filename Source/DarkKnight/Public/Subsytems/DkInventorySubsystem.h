// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkStructs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DkInventorySubsystem.generated.h"

class UDkInventoryComponent;
/**
 * 
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
	UPROPERTY(Transient)
	UDkInventoryComponent* CachedInventoryComponent;

	UPROPERTY(Transient)
	TMap<int, FDkItemInfo> CachedItemTable;

	UPROPERTY(Transient)
	TMap<FName, FDkEntryInfo> CachedEntryTable;

public:
	LIST_DATA_ACCESSOR(UDkInventoryComponent*, CachedInventoryComponent)

	FORCEINLINE TMap<int, FDkItemInfo> GetCachedItemTable() const { return CachedItemTable; }
	FORCEINLINE TMap<FName, FDkEntryInfo> GetCachedEntryTable() const { return CachedEntryTable; }
};
