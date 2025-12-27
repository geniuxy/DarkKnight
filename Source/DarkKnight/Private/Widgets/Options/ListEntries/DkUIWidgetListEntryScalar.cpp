// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/DkUIWidgetListEntryScalar.h"
#include "CommonNumericTextBlock.h"
#include "AnalogSlider.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectScalar.h"

void UDkUIWidgetListEntryScalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AnalogSlider_SettingSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSliderValueChanged);
}

void UDkUIWidgetListEntryScalar::OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningScalarDataObject = CastChecked<UDkUIListDataObjectScalar>(InOwningListDataObject);

	CommonNumeric_SettingValue->SetNumericType(CachedOwningScalarDataObject->GetDisplayNumericType());
	CommonNumeric_SettingValue->FormattingSpecification = CachedOwningScalarDataObject->GetNumberFormattingOptions();
	CommonNumeric_SettingValue->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentDisplayValue());

	AnalogSlider_SettingSlider->SetMinValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetLowerBoundValue());
	AnalogSlider_SettingSlider->SetMaxValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetUpperBoundValue());
	AnalogSlider_SettingSlider->SetStepSize(CachedOwningScalarDataObject->GetSliderStepSize());
	AnalogSlider_SettingSlider->SetValue(CachedOwningScalarDataObject->GetCurrentDisplayValue());
}

void UDkUIWidgetListEntryScalar::OnOwningListDataObjectModified(
	UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningScalarDataObject)
	{
		CommonNumeric_SettingValue->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentDisplayValue());
		AnalogSlider_SettingSlider->SetValue(CachedOwningScalarDataObject->GetCurrentDisplayValue());
	}
}

void UDkUIWidgetListEntryScalar::OnSliderValueChanged(float InNewSliderValue)
{
	if (CachedOwningScalarDataObject)
	{
		CachedOwningScalarDataObject->SetCurrentOutputValue(InNewSliderValue);
	}	
}
