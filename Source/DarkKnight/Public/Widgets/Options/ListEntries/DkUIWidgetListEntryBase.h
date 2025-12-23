// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
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

protected:
	//~Begin IUserObjectListEntry Function
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~End IUserObjectListEntry Function

	// 子类应该重写此函数以处理所需的初始化。需要调用 super 方法。
	virtual void OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject);

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	//***** Bound Widgets *****//
};
