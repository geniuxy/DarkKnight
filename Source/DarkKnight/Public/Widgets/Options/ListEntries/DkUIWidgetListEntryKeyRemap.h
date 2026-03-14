// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/DkUIWidgetListEntryBase.h"
#include "DkUIWidgetListEntryKeyRemap.generated.h"

class UDkUICommonButtonImage;
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
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function
	
	//~Begin UDkUIWidgetListEntryBase Function
	virtual void OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(
		UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason
	) override;
	virtual UWidget* NativeGetWidgetToFocusForGamepad() const override;
	//~End UDkUIWidgetListEntryBase Function

private:
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();

	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyRemapCanceled(const FString& CanceledReason);
	
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonButtonImage* CommonButton_RemapKey;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonButtonBase* CommonButton_ResetKeyBinding;
	//***** Bound Widgets *****//

	UPROPERTY()
	UDkUIListDataObjectKeyRemap* CachedOwningKeyRemapDataObject;
};
