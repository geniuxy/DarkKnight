// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkUIListDataObjectBase.h"
#include "DkUIListDataObjectValue.generated.h"

class FDkUIOptionsDataInteractionHelper;
/**
 * 
 */
UCLASS(Abstract)
class DARKKNIGHT_API UDkUIListDataObjectValue : public UDkUIListDataObjectBase
{
	GENERATED_BODY()

public:
	void SetDataDynamicGetter(const TSharedPtr<FDkUIOptionsDataInteractionHelper>& InDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FDkUIOptionsDataInteractionHelper>& InDynamicSetter);

protected:
	TSharedPtr<FDkUIOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FDkUIOptionsDataInteractionHelper> DataDynamicSetter;
};
