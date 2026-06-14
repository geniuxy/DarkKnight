// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ValueGauge.generated.h"

struct FOnAttributeChangeData;
class UTextBlock;
class UProgressBar;
struct FGameplayAttribute;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UValueGauge : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	void SetAndBoundToGameplayAttribute(
		UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayAttribute& Attribute,
		const FGameplayAttribute& MaxAttribute
	);

	void SetValue(float NewValue, float NewMaxValue);

private:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UProgressBar* ProgressBar;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UTextBlock* ValueText;

	void ValueChanged(const FOnAttributeChangeData& ChangedData);
	void MaxValueChanged(const FOnAttributeChangeData& ChangedData);

	float CachedValue;
	float CachedMaxValue;

	UPROPERTY(EditAnywhere, Category="Visual")
	FLinearColor BarColor;
	
	UPROPERTY(EditAnywhere, Category="Visual")
	FSlateFontInfo ValueTextFont;

	UPROPERTY(EditAnywhere, Category="Visual")
	bool bValueTextVisible = true;

	UPROPERTY(EditAnywhere, Category="Visual")
	bool bProgressBarVisible = true;
};
