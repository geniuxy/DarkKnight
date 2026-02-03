// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryGridSlot.h"
#include "GameplayTagContainer.h"
#include "DkInventoryEquipmentGridSlot.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryEquipmentGridSlot : public UDkInventoryGridSlot
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag EquipmentTypeTag;
};
