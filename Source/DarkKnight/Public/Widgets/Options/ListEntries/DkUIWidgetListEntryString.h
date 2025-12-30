// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/DkUIWidgetListEntryBase.h"
#include "DkUIWidgetListEntryString.generated.h"

class UDkUIListDataObjectString;
class UDkUICommonRotator;
class UDkUICommonButtonBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIWidgetListEntryString : public UDkUIWidgetListEntryBase
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

	//~Begin UDkUIWidgetListEntryBase Function
	virtual void OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(
		UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason
	) override;
	virtual UWidget* NativeGetWidgetToFocusForGamepad() const override;
	virtual void OnToggleEditableState(bool bIsEditable) override;
	//~End UDkUIWidgetListEntryBase Function

private:
	void OnPreviousButtonClicked();
	void OnNextButtonClicked();
	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);

	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonButtonBase* CommonButton_Previous;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonRotator* CommonRotator_AvailableOptions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonButtonBase* CommonButton_Next;
	//***** Bound Widgets *****//

	UPROPERTY(Transient)
	UDkUIListDataObjectString* CachedOwningStringDataObject;
};
