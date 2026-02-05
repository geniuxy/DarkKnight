// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DKWidgetEquipmentMenu.generated.h"

#define NUM_OF_COLUMNS 6

class UDkInventoryEquipmentGridSlot;
class UCanvasPanel;
class UDkInventoryDraggedItem;
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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~End UUserWidget Function

	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> EquipmentCanvasPanel;
	//***** Bound Widgets *****//
	
	bool bMouseWithInCanvas;

	/* 鼠标Hover装备网格，改变其样式 */
	void CalculateHoveredSlot(const FVector2D& CanvasPosition, const FVector2D& MousePosition);
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;

	int32 LastHighlightIndex = INDEX_NONE;

	int32 ItemEquipIndex = INDEX_NONE;
	/********/
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventoryDraggedItem> DraggedItemClass;

	UPROPERTY()
	TObjectPtr<UDkInventoryDraggedItem> DraggedItem;
	
	UFUNCTION()
	void HandleDraggedItemCreated(UDkInventoryDraggedItem* InDraggedItem);
	
	UFUNCTION()
	void HandleDraggedItemRemoved();
	
	UFUNCTION()
	void HandleDraggedItemClicked(const FPointerEvent& MouseEvent);
	
	UPROPERTY()
	TArray<TObjectPtr<UDkInventoryEquipmentGridSlot>> EquippedGridSlots;
};
