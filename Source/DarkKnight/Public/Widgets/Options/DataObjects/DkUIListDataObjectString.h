// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkUIListDataObjectValue.h"
#include "DkUIListDataObjectString.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIListDataObjectString : public UDkUIListDataObjectValue
{
	GENERATED_BODY()

public:
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);
	void SwitchToPreviousOption();
	void SwitchToNextOption();
	void OnRotatorInitiatedValueChanged(const FText& InNewSelectedText);

protected:
	//~Begin UDkUIListDataObjectBase Function
	virtual void OnDataObjectInitialized() override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	//~End UDkUIListDataObjectBase Function

	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);

	FString CurrentStringValue;
	FText CurrentDisplayText;
	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;

public:
	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }
};
