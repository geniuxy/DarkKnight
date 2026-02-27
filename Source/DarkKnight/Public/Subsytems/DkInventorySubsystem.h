// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkKnight/DarkKnight.h"
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

private:
	UPROPERTY(Transient)
	UDkInventoryComponent* CachedInventoryComponent;

public:
	LIST_DATA_ACCESSOR(UDkInventoryComponent*, CachedInventoryComponent)
};
