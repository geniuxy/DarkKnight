// Fill out your copyright notice in the Description page of Project Settings.


#include "DkUIFunctionLibrary.h"

#include "DeveloperSettings/DkUIDeveloperSettings.h"

TSoftClassPtr<UDkWidgetActivatableBase> UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(FGameplayTag InWidgetTag)
{
	const UDkUIDeveloperSettings* UIDeveloperSettings = GetDefault<UDkUIDeveloperSettings>();

	checkf(
		UIDeveloperSettings->UIWidgetMap.Contains(InWidgetTag),
		TEXT("找不到对应的Widget(Tag为%s)"),
		*InWidgetTag.ToString()
	);

	return UIDeveloperSettings->UIWidgetMap.FindRef(InWidgetTag);
}
