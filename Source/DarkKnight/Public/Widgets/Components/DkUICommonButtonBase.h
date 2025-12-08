// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
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
	void SetButtonText(FText InText);

private:
	//~Begin UUserWidget UFunction
	virtual void NativePreConstruct() override;
	//~End UUserWidget UFunction

	/* Begin Bound Widgets */
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonButtonTextBlock;
	/* End Bound Widgets */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDescriptionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	bool bUseUpperCaseForButtonText = false;
};
