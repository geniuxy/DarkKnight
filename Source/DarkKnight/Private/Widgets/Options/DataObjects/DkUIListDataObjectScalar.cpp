// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/DkUIListDataObjectScalar.h"

#include "Widgets/Options/DkUIOptionsDataInteractionHelper.h"

FCommonNumberFormattingOptions UDkUIListDataObjectScalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;

	return Options;
}

FCommonNumberFormattingOptions UDkUIListDataObjectScalar::WithDecimal(int32 NumFracDigit)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigit;

	return Options;
}

float UDkUIListDataObjectScalar::GetCurrentDisplayValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange,
			DisplayValueRange,
			StringToFloat(DataDynamicGetter->GetValueAsString())
		);
	}
	return 0.f;
}

void UDkUIListDataObjectScalar::SetCurrentOutputValue(const float& InNewDisplayValue)
{
	if (DataDynamicSetter)
	{
		float NewOutputValue = FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			OutputValueRange,
			InNewDisplayValue
		);

		DataDynamicSetter->SetValueFromString(LexToString(NewOutputValue));

		NotifyListDataModified(this);
	}
}

bool UDkUIListDataObjectScalar::CanResetBackToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultValue = StringToFloat(GetDefaultValueAsString());
		const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());

		return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f);
	}
	return false;
}

bool UDkUIListDataObjectScalar::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());

			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

			return true;
		}
	}
	return false;
}

void UDkUIListDataObjectScalar::OnEditDependencyDataModified(
	UDkUIListDataObjectBase* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	NotifyListDataModified(this, ModifyReason);
	
	Super::OnEditDependencyDataModified(ModifiedDependencyData, ModifyReason);
}

float UDkUIListDataObjectScalar::StringToFloat(const FString& InString) const
{
	float OutConvertedValue = 0.f;

	LexFromString(OutConvertedValue, InString);

	return OutConvertedValue;
}
