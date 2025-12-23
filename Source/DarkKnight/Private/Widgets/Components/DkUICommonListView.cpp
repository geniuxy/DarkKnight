// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DkUICommonListView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/DkUIOptionListEntryDataMapping.h"
#include "Widgets/Options/ListEntries/DkUIWidgetListEntryBase.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectBase.h"

UUserWidget& UDkUICommonListView::OnGenerateEntryWidgetInternal(
	UObject* Item,
	TSubclassOf<UUserWidget> DesiredEntryClass,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	if (TSubclassOf<UDkUIWidgetListEntryBase> FoundWidgetClass =
		ListEntryDataMapping->FindEntryWidgetClassByDataObject(CastChecked<UDkUIListDataObjectBase>(Item)))
	{
		return GenerateTypedEntry<UDkUIWidgetListEntryBase>(FoundWidgetClass, OwnerTable);
	}
	
	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

void UDkUICommonListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!ListEntryDataMapping)
	{
		CompileLog.Error(FText::FromString(
			TEXT("变量 ListEntryDataMapping 没有指定有效的DataAsset。") +
			GetClass()->GetName() +
			TEXT(" 需要一个有效的DataAsset映射 ListEntry 与 Widget 之间的对应关系才能正常工作")
		));
	}
}
