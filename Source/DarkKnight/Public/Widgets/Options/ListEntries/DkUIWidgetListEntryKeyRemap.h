// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/DkUIWidgetListEntryBase.h"
#include "DkUIWidgetListEntryKeyRemap.generated.h"

class UDkUIListDataObjectKeyRemap;
class UDkUICommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkUIWidgetListEntryKeyRemap : public UDkUIWidgetListEntryBase
{
	GENERATED_BODY()

protected:
	//~Begin UDkUIWidgetListEntryBase Function
	virtual void OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(
		UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason
	) override;
	//~End UDkUIWidgetListEntryBase Function

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonButtonBase* CommonButton_RemapKey;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonButtonBase* CommonButton_ResetKeyBinding;
	//***** Bound Widgets *****//

	UDkUIListDataObjectKeyRemap* CachedOwningKeyRemapDataObject;
};
