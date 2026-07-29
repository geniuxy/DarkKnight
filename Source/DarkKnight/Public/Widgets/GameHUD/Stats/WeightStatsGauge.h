// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatsGauge.h"
#include "WeightStatsGauge.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UWeightStatsGauge : public UStatsGauge
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	float CachedValue;
	float CachedMaxValue;
	
	void SetWeightValue(float NewVal, float NewMaxVal);

	void BaseAttributeChanged(const FOnAttributeChangeData& Data);
	void MaxAttributeChanged(const FOnAttributeChangeData& Data);
};
