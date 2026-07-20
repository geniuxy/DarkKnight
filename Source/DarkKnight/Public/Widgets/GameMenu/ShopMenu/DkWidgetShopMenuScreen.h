// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetShopMenuScreen.generated.h"

class UDkWidgetInventoryMenu;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetShopMenuScreen : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

	//~ Begin UCommonActivatableWidget Function
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	//~ End UCommonActivatableWidget Function

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UCanvasPanel> CanvasPanel_ShopMenu;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkWidgetInventoryMenu> WBP_PlayerInventoryMenu;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkWidgetInventoryMenu> WBP_MerchantInventoryMenu;
	//***** Bound Widgets *****//
};
