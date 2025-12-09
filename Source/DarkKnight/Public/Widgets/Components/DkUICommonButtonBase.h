// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "DkEnums.h"
#include "DkUICommonButtonBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class DARKKNIGHT_API UDkUICommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText, EDkTextJustify InJustification);

private:
	//~Begin UUserWidget UFunction
	virtual void NativePreConstruct() override;
	//~End UUserWidget UFunction

	/* Begin Bound Widgets */
	UPROPERTY(meta=(BindWidgetOptional)) // 用于 C++ 与 UMG 蓝图之间的绑定
	UCommonTextBlock* CommonButtonTextBlock;
	/* End Bound Widgets */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	EDkTextJustify ButtonDisplayTextJustification = EDkTextJustify::Center;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDescriptionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	bool bUseUpperCaseForButtonText = false;
};
