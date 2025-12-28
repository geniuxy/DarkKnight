// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonNumericTextBlock.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectValue.h"
#include "DkUIListDataObjectScalar.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIListDataObjectScalar : public UDkUIListDataObjectValue
{
	GENERATED_BODY()

public:
	LIST_DATA_ACCESSOR(TRange<float>, DisplayValueRange);
	LIST_DATA_ACCESSOR(TRange<float>, OutputValueRange);
	LIST_DATA_ACCESSOR(float, SliderStepSize);
	LIST_DATA_ACCESSOR(ECommonNumericType, DisplayNumericType);
	LIST_DATA_ACCESSOR(FCommonNumberFormattingOptions, NumberFormattingOptions);

	static FCommonNumberFormattingOptions NoDecimal();
	static FCommonNumberFormattingOptions WithDecimal(int32 NumFracDigit);

	float GetCurrentDisplayValue() const;
	void SetCurrentOutputValue(const float& InNewDisplayValue);

protected:
	//~Begin UDkUIListDataObjectBase Function
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	//~End UDkUIListDataObjectBase Function

private:
	float StringToFloat(const FString& InString) const;
	
	TRange<float> DisplayValueRange = TRange<float>(0.f,1.f);
	TRange<float> OutputValueRange = TRange<float>(0.f,1.f);
	float SliderStepSize = 0.1f;
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	FCommonNumberFormattingOptions NumberFormattingOptions;
};
