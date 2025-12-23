// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/DkUIListDataObjectValue.h"

void UDkUIListDataObjectValue::SetDataDynamicGetter(
	const TSharedPtr<FDkUIOptionsDataInteractionHelper>& InDynamicGetter)
{
	DataDynamicGetter = InDynamicGetter;
}

void UDkUIListDataObjectValue::SetDataDynamicSetter(
	const TSharedPtr<FDkUIOptionsDataInteractionHelper>& InDynamicSetter)
{
	DataDynamicSetter = InDynamicSetter;
}
