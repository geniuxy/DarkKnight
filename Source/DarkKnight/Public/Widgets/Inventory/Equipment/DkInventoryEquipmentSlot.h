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

	void SetBrush(FSlateBrush InBrush);

	void SetBackGroundIcon(const TSoftObjectPtr<UTexture2D>& InTexture);
	void SetEquipmentIcon();

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_EquipIcon;
	//***** Bound Widgets *****//
};
