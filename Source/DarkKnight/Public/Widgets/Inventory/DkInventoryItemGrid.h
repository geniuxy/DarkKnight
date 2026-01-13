// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkTypes/DkEnums.h"
#include "DkInventoryItemGrid.generated.h"

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

private:
	void ConstructGrid();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category="Inventory")
	EInventoryItemCategory ItemCategory;

	UPROPERTY()
	TArray<TObjectPtr<UDkInventoryGridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UDkInventoryGridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;

public:
	FORCEINLINE EInventoryItemCategory GetItemCategory() const { return ItemCategory; }
};
