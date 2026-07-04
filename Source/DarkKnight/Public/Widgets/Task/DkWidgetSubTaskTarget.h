// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkTypes/DkStructs.h"
#include "DkWidgetSubTaskTarget.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetSubTaskTarget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void ConfigureSubTaskTarget(
		bool IsFinished, const FText& TargetDescription, int CurSubTaskProgress, int TargetProgress
	);

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonLazyImage* FinishedFlagImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TargetDescriptionText;
	//***** Bound Widgets *****//
};
