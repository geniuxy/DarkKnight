// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetGameMenuScreen.generated.h"

class UDkInventoryComponent;
class UDkInventoryDraggedItem;
class UDKWidgetEquipmentMenu;
class UDkWidgetInventoryMenu;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetGameMenuScreen : public UDkWidgetActivatableBase
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
	TObjectPtr<UDkWidgetInventoryMenu> WBP_InventoryMenu;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDKWidgetEquipmentMenu> WBP_EquipmentMenu;
	//***** Bound Widgets *****//

	TWeakObjectPtr<UDkInventoryDraggedItem> DraggedItem;
	
	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;
	
	UFUNCTION()
	void HandleDraggedItemCreated(UDkInventoryDraggedItem* InDraggedItem);
	
	UFUNCTION()
	void HandleDraggedItemRemoved();

	void OnBackBoundActionTriggered();

public:
	FORCEINLINE UDkWidgetInventoryMenu* GetInventoryMenu() const { return WBP_InventoryMenu; }
};
