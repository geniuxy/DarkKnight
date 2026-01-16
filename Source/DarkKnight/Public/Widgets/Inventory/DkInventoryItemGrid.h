// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkTypes/DkEnums.h"
#include "Inventory/DkInventoryItem.h"
#include "DkInventoryItemGrid.generated.h"

class UDkInventoryItem;
class UDkInventoryComponent;
class UUniformGridPanel;
class UDkInventoryGridSlot;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryItemGrid : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UUniformGridPanel> GridPanel;
	//***** Bound Widgets *****//

	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;

	/* 添加/删除Item */
	UFUNCTION()
	void AddItem(UDkInventoryItem* Item);

	bool MatchesCategory(const UDkInventoryItem* Item) const;
	/********/

	/* 构造Grid */
	virtual void ConstructGrid();

	UPROPERTY()
	TArray<TObjectPtr<UDkInventoryGridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventoryGridSlot> GridSlotClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows = 8;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns = 8;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float SlotDistance = 8.f;
	/********/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	EInventoryItemCategory ItemCategory;

public:
	FORCEINLINE EInventoryItemCategory GetItemCategory() const { return ItemCategory; }
};
