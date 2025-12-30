// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/DkUIWidgetListEntryString.h"

#include "CommonInputSubsystem.h"
#include "Widgets/Components/DkUICommonButtonBase.h"
#include "Widgets/Components/DkUICommonRotator.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectString.h"

void UDkUIWidgetListEntryString::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_Previous->OnClicked().AddUObject(this, &ThisClass::OnPreviousButtonClicked);
	CommonButton_Next->OnClicked().AddUObject(this, &ThisClass::OnNextButtonClicked);

	CommonRotator_AvailableOptions->OnClicked().AddUObject(this, &ThisClass::SelectThisEntryWidget);
	CommonRotator_AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);
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

UWidget* UDkUIWidgetListEntryString::NativeGetWidgetToFocusForGamepad() const
{
	return CommonRotator_AvailableOptions;
}

void UDkUIWidgetListEntryString::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	CommonButton_Previous->SetIsEnabled(bIsEditable);
	CommonRotator_AvailableOptions->SetIsEnabled(bIsEditable);
	CommonButton_Next->SetIsEnabled(bIsEditable);
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

void UDkUIWidgetListEntryString::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (!CachedOwningStringDataObject)
	{
		return;
	}

	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	if (!CommonInputSubsystem || !bUserInitiated)
	{
		return;
	}

	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		CachedOwningStringDataObject->OnRotatorInitiatedValueChanged(CommonRotator_AvailableOptions->GetSelectedText());
	}
}
