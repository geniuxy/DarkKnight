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
	static FIntPoint GetPositionFormIndex(const int32 Index, const int32 Columns);

	UFUNCTION(BlueprintCallable, Category="Inventory Function Library")
	static UDkInventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	template <typename T, typename FuncT>
	static void ForEach2D(
		TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function
	);
};

template <typename T, typename FuncT>
void UDkInventoryFunctionLibrary::ForEach2D(
	TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function)
{
	for (int32 j = 0; j < Range2D.Y; ++j)
	{
		for (int32 i = 0; i < Range2D.X; ++i)
		{
			const FIntPoint Coordinates = GetPositionFormIndex(Index, GridColumns) + FIntPoint(i, j);
			const int32 TileIndex = GetIndexFromPosition(Coordinates, GridColumns);
			if (Array.IsValidIndex(TileIndex))
			{
				Function(Array[TileIndex]);
			}
		}
	}
}
