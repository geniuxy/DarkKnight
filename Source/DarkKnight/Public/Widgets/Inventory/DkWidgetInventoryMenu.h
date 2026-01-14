// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkEnums.h"
#include "DkTypes/DkInventoryStruct.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetInventoryMenu.generated.h"

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
	FDkInventorySlotAvailabilityResult HasRoomForItem(UDkItemComponent* ItemComponent) const;

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UCommonTextBlock> InventoryTitleTxt;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkUICommonButtonBase> Button_Equipment;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkUICommonButtonBase> Button_Consumable;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkUICommonButtonBase> Button_CraftingMaterial;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UCommonLazyImage> SelectedEquipmentUnderline;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UCommonLazyImage> SelectedConsumableUnderline;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UCommonLazyImage> SelectedCraftingMaterialUnderline;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkInventoryItemGrid> GridEquipments;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkInventoryItemGrid> GridConsumables;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkInventoryItemGrid> GridCraftingMaterials;
	//***** Bound Widgets *****//

	/* 切换ItemCategory */
	TMap<EInventoryItemCategory, TObjectPtr<UDkUICommonButtonBase>> CategoryButtonMap;
	TMap<EInventoryItemCategory, TObjectPtr<UCommonLazyImage>> SelectedUnderlineMap;

	void ShowEquipments();
	void ShowConsumables();
	void ShowCraftingMaterials();

	void SelectButton(UDkUICommonButtonBase* Button);
	void ShowSelectedUnderline(UDkInventoryItemGrid* Grid);
	void SetActiveGrid(UDkInventoryItemGrid* Grid, UDkUICommonButtonBase* Button);
	/********/
};
