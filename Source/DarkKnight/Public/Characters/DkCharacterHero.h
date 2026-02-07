// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkCharacterBase.h"
#include "DkCharacterHero.generated.h"

class UDkEquipmentComponent;
class UDkInventoryComponent;

UCLASS()
class DARKKNIGHT_API ADkCharacterHero : public ADkCharacterBase
{
	GENERATED_BODY()

public:
	ADkCharacterHero();

protected:
	/* Actor Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory")
	UDkInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory")
	UDkEquipmentComponent* EquipmentComponent;
	/*********/

public:
	FORCEINLINE UDkInventoryComponent* GetInventoryComponent() const {return InventoryComponent;}
};
