// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"
#include "DkInventoryEquipmentSlot.generated.h"

class UDkInventoryItemDescriptionMenu;
/**
 * 装备物品网格(仅包括网格，不包括装备类标题)
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryEquipmentSlot : public UDkInventoryGridSlot
{
	GENERATED_BODY()

public:
	//~Begin UUserWidget Function
	//~End UUserWidget Function

	void SetBackGroundIcon(const TSoftObjectPtr<UTexture2D>& InTexture);
	void SetEquipmentIcon();
	
	FVector2D GetIconSize() const;

private:
	//***** Bound Widgets *****//
	//***** Bound Widgets *****//
};
