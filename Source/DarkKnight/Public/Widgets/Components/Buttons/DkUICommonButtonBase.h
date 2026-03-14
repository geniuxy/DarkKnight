// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonLazyImage.h"
#include "DkTypes/DkEnums.h"
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
	void SetButtonText(FText InText, EDkTextJustify InJustification = EDkTextJustify::Left);

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;
	
protected:
	//~Begin UUserWidget UFunction
	virtual void NativePreConstruct() override;
	//~End UUserWidget UFunction

	//~Begin UCommonButtonBase UFunction
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	//~End UCommonButtonBase UFunction
	
	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonButtonTextBlock;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button")
	FText ButtonDisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button")
	EDkTextJustify ButtonDisplayTextJustification = EDkTextJustify::Center;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button")
	FText ButtonDescriptionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button")
	bool bUseUpperCaseForButtonText = false;
};
