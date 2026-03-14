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
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~End UUserWidget Function

	void SetBackGroundIcon(const TSoftObjectPtr<UTexture2D>& InTexture);
	void SetEquipmentIcon();
	
	FVector2D GetIconSize() const;

private:
	//***** Bound Widgets *****//
	//***** Bound Widgets *****//

	/* 鼠标Hover时，显示Item详细信息 */
	void CreateItemDescriptionMenu();
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UDkInventoryItemDescriptionMenu> ItemDescriptionMenuClass;

	TWeakObjectPtr<UDkInventoryItemDescriptionMenu> ItemDescriptionMenu;
	/********/
};
