// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/DkUIListDataObjectBase.h"

#include "Settings/DkGameUserSettings.h"

void UDkUIListDataObjectBase::InitDataObject()
{
	OnDataObjectInitialized();
}

void UDkUIListDataObjectBase::AddEditionCondition(const FOptionsDataEditConditionDescriptor& InEditCondition)
{
	EditConditionDescArray.Add(InEditCondition);
}

void UDkUIListDataObjectBase::AddEditionDependencyData(UDkUIListDataObjectBase* InDependencyData)
{
	if (!InDependencyData->OnListDataModified.IsBoundToObject(this))
	{
		InDependencyData->OnListDataModified.AddUObject(this, &UDkUIListDataObjectBase::OnEditDependencyDataModified);
	}
}

bool UDkUIListDataObjectBase::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;

	if (EditConditionDescArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;

	for (const FOptionsDataEditConditionDescriptor& Condition : EditConditionDescArray)
	{
		if (!Condition.IsValid() || Condition.IsEditableCondition())
		{
			continue;
		}

		bIsEditable = false;

		CachedDisabledRichReason.Append(Condition.GetDisabledWarningReason());
		SetWarningRichText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();

			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}

	return bIsEditable;
}

void UDkUIListDataObjectBase::OnDataObjectInitialized()
{
}

void UDkUIListDataObjectBase::NotifyListDataModified(
	UDkUIListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmediately)
	{
		UDkGameUserSettings::Get()->ApplySettings(true);
	}
}

void UDkUIListDataObjectBase::OnEditDependencyDataModified(
	UDkUIListDataObjectBase* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason)
{
	OnDependencyDataModified.Broadcast(ModifiedDependencyData, ModifyReason);
}

void UDkUIListDataObjectBase::OnSetToForcedStringValue(const FString& InForcedValue)
{
}
