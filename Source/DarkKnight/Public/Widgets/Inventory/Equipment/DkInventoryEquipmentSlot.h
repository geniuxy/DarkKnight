// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/Inventory/DkInventoryGridSlot.h"
#include "DkInventoryEquipmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryEquipmentSlot : public UDkInventoryGridSlot
{
	GENERATED_BODY()

public:
	//~Begin UUserWidget Function
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	//~End UUserWidget Function

	virtual void SetOccupiedTexture() override;
	virtual void SetEnabledBrush();
	virtual void SetDisabledBrush();
	virtual void SetGrayedOutTexture() override;

protected:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush NormalBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush EnabledBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush DisabledBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush GrayedOutBrush;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "Dk.Item.Equipment"))
	FGameplayTag EquipmentTypeTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, EquipmentTypeTag)
};
