// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkUIOptionsDataInteractionHelper.h"

#include "Settings/DkGameUserSettings.h"

FDkUIOptionsDataInteractionHelper::FDkUIOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath)
	: CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	CachedWeakGameUserSettings = UDkGameUserSettings::Get();
}

FString FDkUIOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(
		CachedWeakGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		OutStringValue
	);

	return OutStringValue;
}

void FDkUIOptionsDataInteractionHelper::SetValueFromString(const FString& InStringValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedWeakGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		InStringValue
	);
}
