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
}

void UDkUIWidgetListEntryString::OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UDkUIListDataObjectString>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}

void UDkUIWidgetListEntryString::OnPreviousButtonClicked()
{
	Debug::Print(TEXT("上一个选项按钮被点击"));
}

void UDkUIWidgetListEntryString::OnNextButtonClicked()
{
	Debug::Print(TEXT("下一个选项按钮被点击"));
}
