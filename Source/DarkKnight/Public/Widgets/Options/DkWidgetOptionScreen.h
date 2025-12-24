// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetOptionScreen.generated.h"

class UDkUITabListWidgetBase;
class UDkUICommonListView;
class UDkUIOptionsDataRegistry;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkWidgetOptionScreen : public UDkWidgetActivatableBase
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
	UDkUIOptionsDataRegistry* GetOrCreateDataRegistry();
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabId);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	UDkUITabListWidgetBase* TabListWidget_OptionsTabs;
	
	UPROPERTY(meta = (BindWidget))
	UDkUICommonListView* CommonListView_OptionsList;
	//***** Bound Widgets *****//

	// 在OptionScreen中处理数据创建。禁止直接访问此变量。
	UPROPERTY(Transient)
	UDkUIOptionsDataRegistry* CreatedOwningDataRegistry;

	UPROPERTY(EditDefaultsOnly, Category = "Dk Options Screen", meta=(RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;
};
