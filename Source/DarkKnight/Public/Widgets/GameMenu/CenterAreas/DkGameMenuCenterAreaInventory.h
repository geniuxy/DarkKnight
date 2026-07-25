// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "Widgets/Inventory/DkInventoryItemDescriptionMenu.h"
#include "DkGameMenuCenterAreaInventory.generated.h"

class UDkPlayerInventoryComp;
class UCanvasPanel;
class UDkInventoryComponent;
class UDkInventoryDraggedItem;
class UDKWidgetEquipmentMenu;
class UDkWidgetInventoryMenu;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkGameMenuCenterAreaInventory : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~End UUserWidget Function

	//~ Begin UCommonActivatableWidget Function
	virtual void NativeOnDeactivated() override;
	//~ End UCommonActivatableWidget Function

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkWidgetInventoryMenu> WBP_InventoryMenu;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDKWidgetEquipmentMenu> WBP_EquipmentMenu;
	//***** Bound Widgets *****//

	TWeakObjectPtr<UDkInventoryDraggedItem> DraggedItem;
	
	TWeakObjectPtr<UDkPlayerInventoryComp> InventoryComponent;
	
	UFUNCTION()
	void HandleDraggedItemCreated(UDkInventoryDraggedItem* InDraggedItem);
	
	UFUNCTION()
	void HandleDraggedItemRemoved();

	TWeakObjectPtr<UDkInventoryItemDescriptionMenu> ItemDescriptionMenu;
	
	UFUNCTION()
	void HandleItemDescriptionCreated(UDkInventoryItemDescriptionMenu* InItemDescriptionMenu);

	UFUNCTION()
	void HandleItemDescriptionRemoved();

public:
	FORCEINLINE UDkWidgetInventoryMenu* GetInventoryMenu() const { return WBP_InventoryMenu; }
};
