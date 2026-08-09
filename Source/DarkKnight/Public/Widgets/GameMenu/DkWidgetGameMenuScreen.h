// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetGameMenuScreen.generated.h"

class UDkGameMenuCenterAreaCraft;
class UDkInventoryComponent;
class UDkGameMenuCenterAreaTask;
struct FGameplayTag;
class UDkWidgetInventoryMenu;
class UDkGameMenuCenterAreaInventory;
class UCommonVisibilitySwitcher;
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

	//~ Begin UCommonActivatableWidget Function
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	//~ End UCommonActivatableWidget Function

	UDkWidgetInventoryMenu* GetInventoryMenu() const;

	void SetVisibleCenterArea(const FGameplayTag& InTag) const;

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkGameMenuCenterAreaInventory> CenterArea_Inventory;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkWidgetActivatableBase> CenterArea_Map;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkGameMenuCenterAreaTask> CenterArea_Task;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UDkGameMenuCenterAreaCraft> CenterArea_Craft;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UCommonVisibilitySwitcher> CommonVisibilitySwitcher;
	//***** Bound Widgets *****//

	UDkWidgetActivatableBase* GetActivatableCenterAreaWidget(const FGameplayTag& InTag) const;

	void OnBackBoundActionTriggered();
};
