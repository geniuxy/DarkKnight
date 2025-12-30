// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DkTypes/DkEnums.h"
#include "DkUIWidgetListEntryBase.generated.h"

class UDkUIListDataObjectBase;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkUIWidgetListEntryBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);

	void NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:
	//~Begin IUserObjectListEntry Function
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override; // 用于切换Tab后，将每一个设置项都先置为Unhovered状态
	//~End IUserObjectListEntry Function

	//~Begin UUserWidget Function
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//~End UUserWidget Function

	virtual UWidget* NativeGetWidgetToFocusForGamepad() const;
	// 蓝图应该重写此函数，以便游戏手柄交互能够正常工作。
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;
	
	// 子类应该重写此函数以处理所需的初始化。需要调用 super 方法。
	virtual void OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject);

	// 子类应该重写此函数，以便在ListDataObject修改后更新 UI 值。无需调用super方法。
	virtual void OnOwningListDataObjectModified(
		UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason
	);

	// 子类应该重写此函数，以更改其拥有的控件的可编辑状态,需要调用 super 方法。
	virtual void OnToggleEditableState(bool bIsEditable);

	void SelectThisEntryWidget();

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	//***** Bound Widgets *****//
};
