// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/DkUIWidgetListEntryScalar.h"
#include "CommonNumericTextBlock.h"
#include "AnalogSlider.h"

void UDkUIWidgetListEntryScalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UDkUIWidgetListEntryScalar::OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
}

void UDkUIWidgetListEntryScalar::OnOwningListDataObjectModified(
	UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
}
