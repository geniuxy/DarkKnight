// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/DkUIWidgetListEntryString.h"

#include "DarkKnightDebugHelper.h"
#include "Widgets/Components/DkUICommonButtonBase.h"
#include "Widgets/Components/DkUICommonRotator.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectString.h"

void UDkUIWidgetListEntryString::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_Previous->OnClicked().AddUObject(this, &ThisClass::OnPreviousButtonClicked);
	CommonButton_Next->OnClicked().AddUObject(this, &ThisClass::OnNextButtonClicked);

	CommonRotator_AvailableOptions->OnClicked().AddUObject(this, &ThisClass::SelectThisEntryWidget);
}

void UDkUIWidgetListEntryString::OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UDkUIListDataObjectString>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}

void UDkUIWidgetListEntryString::OnOwningListDataObjectModified(
	UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UDkUIWidgetListEntryString::OnPreviousButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->SwitchToPreviousOption();
	}

	SelectThisEntryWidget();
}

void UDkUIWidgetListEntryString::OnNextButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->SwitchToNextOption();
	}

	SelectThisEntryWidget();
}
