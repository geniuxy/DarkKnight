// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/DkUIListDataObjectStrResolution.h"

#include "DarkKnightDebugHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Options/DkUIOptionsDataInteractionHelper.h"

void UDkUIListDataObjectStrResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolution;

	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolution);

	for (const FIntPoint& Resolution : AvailableResolution)
	{
		Debug::Print(TEXT("可用的分辨率：") + Resolution.ToString());
	}
}

void UDkUIListDataObjectStrResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (DataDynamicGetter)
	{
		Debug::Print(TEXT("从 DynamicGetter 中读取的分辨率值：") + DataDynamicGetter->GetValueAsString());
	}
}
