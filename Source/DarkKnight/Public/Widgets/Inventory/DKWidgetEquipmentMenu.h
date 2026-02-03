// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DKWidgetEquipmentMenu.generated.h"

class UDkInventoryDraggedItem;
class UDkInventoryEquipmentGridSlot;
class UDkInventoryComponent;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDKWidgetEquipmentMenu : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventoryDraggedItem> DraggedItemClass;

	UPROPERTY()
	TObjectPtr<UDkInventoryDraggedItem> DraggedItem;
	
	UFUNCTION()
	void HandleDraggedItemCreated(UDkInventoryDraggedItem* InDraggedItem);
	
	UFUNCTION()
	void HandleDraggedItemClicked(const FPointerEvent& MouseEvent);
	
	UPROPERTY()
	TArray<TObjectPtr<UDkInventoryEquipmentGridSlot>> EquippedGridSlots;
};
