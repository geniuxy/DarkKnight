// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"

#include "Characters/DkCharacterHero.h"
#include "Components/DkInventoryComponent.h"

int32 UDkInventoryFunctionLibrary::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

UDkInventoryComponent* UDkInventoryFunctionLibrary::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;
	UDkInventoryComponent* InventoryComponent =
		CastChecked<ADkCharacterHero>(PlayerController->GetPawn())->FindComponentByClass<UDkInventoryComponent>();
	return InventoryComponent;
}
