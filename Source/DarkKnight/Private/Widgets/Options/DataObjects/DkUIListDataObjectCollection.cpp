// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/DkUIListDataObjectCollection.h"

void UDkUIListDataObjectCollection::AddChildListData(UDkUIListDataObjectBase* InChildListData)
{
	InChildListData->InitDataObject();

	InChildListData->SetParentData(this);
	
	ChildListDataArray.Add(InChildListData);
}

TArray<UDkUIListDataObjectBase*> UDkUIListDataObjectCollection::GetAllChildSettingData() const
{
	return ChildListDataArray;
}

bool UDkUIListDataObjectCollection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}
