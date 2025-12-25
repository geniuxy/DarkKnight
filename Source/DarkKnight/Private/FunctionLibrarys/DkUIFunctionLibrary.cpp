// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkUIFunctionLibrary.h"

#include "Settings/DkUIDeveloperSettings.h"

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

TSoftObjectPtr<UTexture2D> UDkUIFunctionLibrary::GetSoftImageByTag(FGameplayTag InImageTag)
{
	const UDkUIDeveloperSettings* UIDeveloperSettings = GetDefault<UDkUIDeveloperSettings>();

	checkf(
		UIDeveloperSettings->ImageMap.Contains(InImageTag),
		TEXT("找不到对应的Image(Tag为%s)"),
		*InImageTag.ToString()
	);

	return UIDeveloperSettings->ImageMap.FindRef(InImageTag);
}
