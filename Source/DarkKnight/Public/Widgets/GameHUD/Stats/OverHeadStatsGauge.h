// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "OverHeadStatsGauge.generated.h"

class UValueGauge;
struct FOnAttributeChangeData;
class UTextBlock;
class UProgressBar;
struct FGameplayAttribute;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UOverHeadStatsGauge : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void ConfigureWithASC(UAbilitySystemComponent* AbilitySystemComponent);

private:
	UPROPERTY(meta=(BindWidget))
	UValueGauge* HealthBar;
	
	UPROPERTY(meta=(BindWidget))
	UValueGauge* EnergyBar;
};
