// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetSystemMenuScreen.generated.h"

class UDkUICommonButtonImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetSystemMenuScreen : public UDkWidgetActivatableBase
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
	TObjectPtr<UDkUICommonButtonImage> SelectButton_Craft;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkUICommonButtonImage> SelectButton_Alchemy;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkUICommonButtonImage> SelectButton_Inventory;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkUICommonButtonImage> SelectButton_Map;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkUICommonButtonImage> SelectButton_Task;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkUICommonButtonImage> SelectButton_Meditation;
	//***** Bound Widgets *****//

	void HandleSelectButtonInventory();
	void HandleSelectButtonMap();
	void HandleSelectButtonTask();

	void OnBackBoundActionTriggered();
};
