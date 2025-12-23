// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UDkGameUserSettings;
/**
 * 
 */
class DARKKNIGHT_API FDkUIOptionsDataInteractionHelper
{
public:
	FDkUIOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath);

	FString GetValueAsString() const;
	void SetValueFromString(const FString& InStringValue);

private:
	FCachedPropertyPath CachedDynamicFunctionPath;
	TWeakObjectPtr<UDkGameUserSettings> CachedWeakGameUserSettings;
};
