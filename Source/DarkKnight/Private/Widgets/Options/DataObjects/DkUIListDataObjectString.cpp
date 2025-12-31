// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/DkUIListDataObjectString.h"

#include "DarkKnightDebugHelper.h"
#include "Widgets/Options/DkUIOptionsDataInteractionHelper.h"

void UDkUIListDataObjectString::AddDynamicOption(const FString& InStringValue, const FText& InDisplayText)
{
	AvailableOptionsStringArray.Add(InStringValue);
	AvailableOptionsTextArray.Add(InDisplayText);
}

void UDkUIListDataObjectString::SwitchToPreviousOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousIndexToDisplay = CurrentDisplayIndex - 1;

	if (AvailableOptionsStringArray.IsValidIndex(PreviousIndexToDisplay))
	{
		CurrentStringValue = AvailableOptionsStringArray[PreviousIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray.Last();
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		Debug::Print(TEXT("DataDynamicSetter已使用. 最新可从Getter中得到的值为: ") + DataDynamicGetter->GetValueAsString());
	}

	NotifyListDataModified(this);
}

void UDkUIListDataObjectString::SwitchToNextOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;

	if (AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay))
	{
		CurrentStringValue = AvailableOptionsStringArray[NextIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);

		Debug::Print(TEXT("DataDynamicSetter已使用. 最新可从Getter中得到的值为: ") + DataDynamicGetter->GetValueAsString());
	}

	NotifyListDataModified(this);
}

void UDkUIListDataObjectString::OnRotatorInitiatedValueChanged(const FText& InNewSelectedText)
{
	const int32 FoundIndex = AvailableOptionsTextArray.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)-> bool
		{
			return AvailableText.EqualTo(InNewSelectedText);
		}
	);

	if (FoundIndex != INDEX_NONE && AvailableOptionsStringArray.IsValidIndex(FoundIndex))
	{
		CurrentStringValue = AvailableOptionsStringArray[FoundIndex];
		CurrentDisplayText = InNewSelectedText;

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);

			Debug::Print(
				TEXT("手柄修改了设置选项 ") +
				GetDataDisplayName().ToString() +
				TEXT(" ，DataDynamicSetter已使用. 最新可从Getter中得到的值为: ") +
				DataDynamicGetter->GetValueAsString()
			);
		}

		NotifyListDataModified(this);
	}
}

void UDkUIListDataObjectString::OnDataObjectInitialized()
{
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}

	// 读取保存的StringValue and set CurrentStringValue
	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("无效选项"));
	}
}

bool UDkUIListDataObjectString::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UDkUIListDataObjectString::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();

		TrySetDisplayTextFromStringValue(CurrentStringValue);

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);

			Debug::Print(
				TEXT("重置为默认值，DataDynamicSetter已使用. 最新可从Getter中得到的值为: ") +
				DataDynamicGetter->GetValueAsString()
			);
		}

		NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

		return true;
	}
	return false;
}

bool UDkUIListDataObjectString::CanSetToForcedStringValue(const FString& InForcedValue) const
{
	return CurrentStringValue != InForcedValue;
}

void UDkUIListDataObjectString::OnSetToForcedStringValue(const FString& InForcedValue)
{
	CurrentStringValue = InForcedValue;
	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
	}

	NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
}

bool UDkUIListDataObjectString::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	const int32 CurrentFoundIndex = AvailableOptionsStringArray.IndexOfByKey(InStringValue);

	if (AvailableOptionsTextArray.IsValidIndex(CurrentFoundIndex))
	{
		CurrentDisplayText = AvailableOptionsTextArray[CurrentFoundIndex];

		return true;
	}

	return false;
}

//************ UDkUIListDataObjectStringBool ************//

void UDkUIListDataObjectStringBool::OverrideTrueDisplayText(const FText& InNewTrueDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString, InNewTrueDisplayText);
	}
}

void UDkUIListDataObjectStringBool::OverrideFalseDisplayText(const FText& InNewFalseDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, InNewFalseDisplayText);
	}
}

void UDkUIListDataObjectStringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(TrueString);
}

void UDkUIListDataObjectStringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(FalseString);
}

void UDkUIListDataObjectStringBool::OnDataObjectInitialized()
{
	TryInitBoolValues();

	Super::OnDataObjectInitialized();
}

void UDkUIListDataObjectStringBool::TryInitBoolValues()
{
	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString, FText::FromString(TEXT("开")));
	}

	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, FText::FromString(TEXT("关")));
	}
}

//************ UDkUIListDataObjectStringBool ************//

//************ UDkUIListDataObjectStringInteger ************//

void UDkUIListDataObjectStringInteger::AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText)
{
	AddDynamicOption(LexToString(InIntegerValue), InDisplayText);
}

void UDkUIListDataObjectStringInteger::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Custom"));
	}
}

void UDkUIListDataObjectStringInteger::OnEditDependencyDataModified(
	UDkUIListDataObjectBase* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	if (DataDynamicGetter)
	{
		CurrentStringValue = DataDynamicGetter->GetValueAsString();

		if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
		{
			CurrentDisplayText = FText::FromString(TEXT("Custom"));
		}

		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
	}
	
	Super::OnEditDependencyDataModified(ModifiedDependencyData, ModifyReason);
}

//************ UDkUIListDataObjectStringInteger ************//
