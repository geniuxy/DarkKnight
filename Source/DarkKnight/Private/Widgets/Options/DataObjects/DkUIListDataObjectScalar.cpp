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

float UDkUIListDataObjectScalar::StringToFloat(const FString& InString) const
{
	float OutConvertedValue = 0.f;

	LexFromString(OutConvertedValue, InString);

	return OutConvertedValue;
}
