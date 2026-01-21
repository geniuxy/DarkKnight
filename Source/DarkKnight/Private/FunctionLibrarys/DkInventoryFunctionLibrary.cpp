// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Characters/DkCharacterHero.h"
#include "Components/DkInventoryComponent.h"
#include "Components/Widget.h"

int32 UDkInventoryFunctionLibrary::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint UDkInventoryFunctionLibrary::GetPositionFormIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

UDkInventoryComponent* UDkInventoryFunctionLibrary::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;
	UDkInventoryComponent* InventoryComponent =
		CastChecked<ADkCharacterHero>(PlayerController->GetPawn())->FindComponentByClass<UDkInventoryComponent>();
	return InventoryComponent;
}

FVector2D UDkInventoryFunctionLibrary::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(
		Widget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry), PixelPosition, ViewportPosition
	);
	return ViewportPosition;
}
