// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DkUITabListWidgetBase.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Components/DkUICommonButtonBase.h"

void UDkUITabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("变量 TabButtonEntryWidgetClass 没有指定有效的条目类。") +
			GetClass()->GetName() +
			TEXT(" 需要一个有效的条目控件类才能正常工作")
		));
	}
}
