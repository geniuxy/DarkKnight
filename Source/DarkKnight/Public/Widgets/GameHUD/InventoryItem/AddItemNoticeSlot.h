// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "AddItemNoticeSlot.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UAddItemNoticeSlot : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void ConfigureNoticeText(const FText& InText, float InExistTime);

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* NoticeTextBlock;
	//***** Bound Widgets *****//
};
