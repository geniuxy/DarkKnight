// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/DkUIWidgetListEntryString.h"

#include "Widgets/Components/DkUICommonRotator.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectString.h"

void UDkUIWidgetListEntryString::OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UDkUIListDataObjectString>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}
