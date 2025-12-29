// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/DkUIListDataObjectStrResolution.h"

#include "DarkKnightDebugHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Options/DkUIOptionsDataInteractionHelper.h"

void UDkUIListDataObjectStrResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;

	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B)
		{
			return std::tie(A.X, A.Y) < std::tie(B.X, B.Y);// 先按x从小到大进行排列，再按y从小到大进行排列
		}
	);

	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResToStringValue(Resolution), ResToDisplayText(Resolution));
	}

	MaxAllowedResolution = ResToStringValue(AvailableResolutions.Last());
	
	SetDefaultValueFromString(MaxAllowedResolution);
}

void UDkUIListDataObjectStrResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (DataDynamicGetter)
	{
		Debug::Print(TEXT("从 DynamicGetter 中读取的当前分辨率值：") + DataDynamicGetter->GetValueAsString());
	}
}

FString UDkUIListDataObjectStrResolution::ResToStringValue(const FIntPoint& InResolution) const
{
	return FString::Printf(TEXT("(X=%i,Y=%i)"),InResolution.X,InResolution.Y);
}

FText UDkUIListDataObjectStrResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%i x %i"),InResolution.X,InResolution.Y);

	return FText::FromString(DisplayString);
}
