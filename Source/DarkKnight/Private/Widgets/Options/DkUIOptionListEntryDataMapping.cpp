// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkUIOptionListEntryDataMapping.h"

#include "Widgets/Options/DataObjects/DkUIListDataObjectBase.h"

TSubclassOf<UDkUIWidgetListEntryBase> UDkUIOptionListEntryDataMapping::FindEntryWidgetClassByDataObject(
	UDkUIListDataObjectBase* InDataObject) const
{
	check(InDataObject);

	// 子类所用widget可能与父类相同，因此遍历的时候需要遍历其父类
	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass;
	     DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if (TSubclassOf<UDkUIListDataObjectBase> ConvertedDataObjectClass =
			TSubclassOf<UDkUIListDataObjectBase>(DataObjectClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}

	return TSubclassOf<UDkUIWidgetListEntryBase>();
}
