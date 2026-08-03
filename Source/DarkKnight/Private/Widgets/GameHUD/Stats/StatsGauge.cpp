// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/Stats/StatsGauge.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Characters/DkCharacterBase.h"

void UStatsGauge::NativePreConstruct()
{
	Super::NativePreConstruct();
	Icon->SetBrushFromTexture(IconTexture);
	Icon->SetColorAndOpacity(IconColor);
}

void UStatsGauge::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatsGauge::SetOwner(AActor* OwnerCharacter)
{
	StatsOwner = OwnerCharacter;
	
	InitAttributeChangeCallback();
}

void UStatsGauge::InitAttributeChangeCallback()
{
	if (!StatsOwner) return;

	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(StatsOwner);
	if (OwnerASC)
	{
		bool bFound;
		float AttributeVal = OwnerASC->GetGameplayAttributeValue(Attribute, bFound);
		SetValue(AttributeVal);

		OwnerASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(
			this, &ThisClass::AttributeChanged
		);
	}
}

void UStatsGauge::SetValue(float NewVal)
{
	FNumberFormattingOptions NumberFormattingOptions;
	NumberFormattingOptions.MaximumFractionalDigits = 0;

	AttributeText->SetText(FText::AsNumber(NewVal, &NumberFormattingOptions));
}

void UStatsGauge::AttributeChanged(const FOnAttributeChangeData& Data)
{
	SetValue(Data.NewValue);
}
