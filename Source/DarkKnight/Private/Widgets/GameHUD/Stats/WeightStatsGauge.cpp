// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/Stats/WeightStatsGauge.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CommonTextBlock.h"
#include "GAS/DkAttributeSet.h"

void UWeightStatsGauge::NativeConstruct()
{
	UCommonUserWidget::NativeConstruct();
}

void UWeightStatsGauge::InitAttributeChangeCallback()
{
	if (!StatsOwner) return;

	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(StatsOwner);
	if (OwnerASC)
	{
		bool bFound;
		float CarryWeightVal = OwnerASC->GetGameplayAttributeValue(UDkAttributeSet::GetCarryWeightAttribute(), bFound);
		float WeightLimitVal = OwnerASC->GetGameplayAttributeValue(UDkAttributeSet::GetWeightLimitAttribute(), bFound);
		SetWeightValue(CarryWeightVal, WeightLimitVal);

		OwnerASC->GetGameplayAttributeValueChangeDelegate(UDkAttributeSet::GetCarryWeightAttribute()).AddUObject(
			this, &ThisClass::BaseAttributeChanged
		);
		OwnerASC->GetGameplayAttributeValueChangeDelegate(UDkAttributeSet::GetWeightLimitAttribute()).AddUObject(
			this, &ThisClass::MaxAttributeChanged
		);
	}
}

void UWeightStatsGauge::SetWeightValue(float NewVal, float NewMaxVal)
{
	CachedValue = NewVal;
	CachedMaxValue = NewMaxVal;
	
	if (NewMaxVal == 0.f)
	{
		return;
	}

	FNumberFormattingOptions FormatOps = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	FormatOps.UseGrouping = false;

	AttributeText->SetText(
		FText::Format(
			FTextFormat::FromString("{0}/{1}"),
			FText::AsNumber(NewVal, &FormatOps),
			FText::AsNumber(NewMaxVal, &FormatOps)
		)
	);
}

void UWeightStatsGauge::BaseAttributeChanged(const FOnAttributeChangeData& Data)
{
	SetWeightValue(Data.NewValue, CachedMaxValue);
}

void UWeightStatsGauge::MaxAttributeChanged(const FOnAttributeChangeData& Data)
{
	SetWeightValue(CachedValue, Data.NewValue);
}
