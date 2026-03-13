// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetGameMenuScreen.generated.h"

class UDkWidgetInventoryMenu;
class UDkGameMenuCenterAreaInventory;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetGameMenuScreen : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

public:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function
	
	UDkWidgetInventoryMenu* GetInventoryMenu() const;

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkGameMenuCenterAreaInventory> CenterArea_Inventory;
	//***** Bound Widgets *****//

	void OnBackBoundActionTriggered();
public:
};
