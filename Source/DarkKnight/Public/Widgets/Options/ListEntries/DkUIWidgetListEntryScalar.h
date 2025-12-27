// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/DkUIWidgetListEntryBase.h"
#include "DkUIWidgetListEntryScalar.generated.h"

class UDkUIListDataObjectScalar;
class UCommonNumericTextBlock;
class UAnalogSlider;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkUIWidgetListEntryScalar : public UDkUIWidgetListEntryBase
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
	//~End UDkUIWidgetListEntryBase Function
private:
	UFUNCTION()
	void OnSliderValueChanged(float InNewSliderValue);

	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UAnalogSlider* AnalogSlider_SettingSlider;
	//***** Bound Widgets *****//

	UPROPERTY(Transient)
	UDkUIListDataObjectScalar* CachedOwningScalarDataObject;
};
