// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DkInventoryFunctionLibrary.generated.h"

class UDkInventoryComponent;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Inventory Function Library")
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);
	
	UFUNCTION(BlueprintCallable, Category="Inventory Function Library")
	static UDkInventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);
};
