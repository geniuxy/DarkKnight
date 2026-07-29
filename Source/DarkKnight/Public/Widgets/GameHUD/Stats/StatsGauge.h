// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "CommonUserWidget.h"
#include "StatsGauge.generated.h"

struct FOnAttributeChangeData;
class UCommonTextBlock;
class UCommonLazyImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UStatsGauge : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* Icon;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* AttributeText;

	UPROPERTY(EditAnywhere, Category="Attribute")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, Category="Visual")
	UTexture2D* IconTexture;

	UPROPERTY(EditAnywhere, Category="Visual")
	FLinearColor IconColor;

	void SetValue(float NewVal);

	void AttributeChanged(const FOnAttributeChangeData& Data);
};
