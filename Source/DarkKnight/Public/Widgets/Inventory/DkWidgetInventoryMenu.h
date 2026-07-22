// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkEnums.h"
#include "Inventory/DkInventorySlotAvailabilty.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetInventoryMenu.generated.h"

class UDkInventoryComponent;
class UDkUICommonButtonImage;
class UDkItemComponent;
class UCommonLazyImage;
class UDkUICommonButtonBase;
class UDkInventoryItemGrid;
class UWidgetSwitcher;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkWidgetInventoryMenu : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual FDkInventorySlotAvailabilityResult HasRoomForItem(UDkItemComponent* ItemComponent) const;
	virtual FDkInventorySlotAvailabilityResult HasRoomForItem(UDkInventoryItem* Item) const;

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> InventoryTitleTxt;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDkUICommonButtonImage> Button_Equipment;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDkUICommonButtonImage> Button_Consumable;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDkUICommonButtonImage> Button_CraftingMaterial;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonLazyImage> SelectedEquipmentUnderline;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonLazyImage> SelectedConsumableUnderline;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonLazyImage> SelectedCraftingMaterialUnderline;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDkInventoryItemGrid> GridEquipments;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDkInventoryItemGrid> GridConsumables;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDkInventoryItemGrid> GridCraftingMaterials;
	//***** Bound Widgets *****//

	/* 切换ItemCategory */
	TMap<EInventoryItemCategory, TObjectPtr<UDkUICommonButtonImage>> CategoryButtonMap;
	TMap<EInventoryItemCategory, TObjectPtr<UCommonLazyImage>> SelectedUnderlineMap;

	TWeakObjectPtr<UDkInventoryItemGrid> ActiveGrid;

	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;

	void ShowEquipments();
	void ShowConsumables();
	void ShowCraftingMaterials();

	void SelectButton(UDkUICommonButtonImage* Button);
	void ShowSelectedUnderline(UDkInventoryItemGrid* Grid);
	void SetActiveGrid(UDkInventoryItemGrid* Grid, UDkUICommonButtonImage* Button);

public:
	TArray<UDkInventoryItemGrid*> GetAllInventoryItemGrid() const;
	/********/
};
